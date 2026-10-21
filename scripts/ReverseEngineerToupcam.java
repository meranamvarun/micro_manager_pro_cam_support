// Read-only Ghidra headless script for the ToupCam Micro-Manager adapter.
// Usage: -postScript ReverseEngineerToupcam.java OUTPUT [NEEDLES] [MAX_FUNCTIONS]

import java.io.File;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressIterator;
import ghidra.program.model.data.StringDataInstance;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.DataIterator;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.program.model.symbol.Symbol;
import ghidra.program.model.symbol.SymbolIterator;
import ghidra.program.model.symbol.SymbolTable;
import ghidra.util.task.ConsoleTaskMonitor;

public class ReverseEngineerToupcam extends GhidraScript {
    private PrintWriter out;

    @Override
    protected void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 1) {
            throw new IllegalArgumentException("Expected output path");
        }

        int needleEnd = args.length;
        int maxFunctions = 100;
        if (args.length > 1 && args[args.length - 1].matches("[0-9]+")) {
            maxFunctions = Integer.parseInt(args[args.length - 1]);
            needleEnd--;
        }
        List<String> needles = new ArrayList<>();
        for (int i = 1; i < needleEnd; i++) {
            needles.addAll(parseNeedles(args[i]));
        }
        if (needles.isEmpty()) {
            needles = parseNeedles(
                "Dll,GetProcAddress,LoadLibrary,Toupcam,PullImage,EnumCamera");
        }
        File output = new File(args[0]);
        File parent = output.getParentFile();
        if (parent != null) {
            parent.mkdirs();
        }

        try (PrintWriter writer = new PrintWriter(output, "UTF-8")) {
            out = writer;
            writeIdentity();
            writeImports();
            Set<Function> selected = new LinkedHashSet<>();
            writeExports(needles, selected);
            writeRelevantStrings(needles, selected);
            writeDecompilations(selected, maxFunctions);
        }

        println("Wrote reverse-engineering evidence to " + output.getAbsolutePath());
    }

    private List<String> parseNeedles(String raw) {
        List<String> values = new ArrayList<>();
        for (String value : raw.split("[,;]")) {
            String trimmed = value.trim();
            if (!trimmed.isEmpty()) {
                values.add(trimmed);
            }
        }
        return values;
    }

    private boolean matches(String value, List<String> needles) {
        String lower = value.toLowerCase();
        for (String needle : needles) {
            if (lower.contains(needle.toLowerCase())) {
                return true;
            }
        }
        return false;
    }

    private void writeIdentity() {
        out.println("# Direct Ghidra static-analysis evidence");
        out.println();
        out.println("- Executable: `" + currentProgram.getExecutablePath() + "`");
        out.println("- SHA-256: `" + currentProgram.getExecutableSHA256() + "`");
        out.println("- Format: `" + currentProgram.getExecutableFormat() + "`");
        out.println("- Language: `" + currentProgram.getLanguageID() + "`");
        out.println("- Image base: `" + currentProgram.getImageBase() + "`");
        out.println();
    }

    private void writeImports() {
        out.println("## Imported symbols");
        out.println();
        SymbolIterator symbols = currentProgram.getSymbolTable().getExternalSymbols();
        while (symbols.hasNext()) {
            Symbol symbol = symbols.next();
            out.println("- `" + symbol.getName(true) + "`");
        }
        out.println();
    }

    private void writeExports(List<String> needles, Set<Function> selected) {
        out.println("## Exported entry points");
        out.println();
        SymbolTable table = currentProgram.getSymbolTable();
        AddressIterator addresses = table.getExternalEntryPointIterator();
        while (addresses.hasNext()) {
            Address address = addresses.next();
            Symbol symbol = table.getPrimarySymbol(address);
            String name = symbol == null ? "<unnamed>" : symbol.getName(true);
            out.println("- `" + address + "` `" + name + "`");
            if (matches(name, needles)) {
                Function function = currentProgram.getFunctionManager().getFunctionAt(address);
                if (function != null) {
                    selected.add(function);
                }
            }
        }
        out.println();
    }

    private void writeRelevantStrings(List<String> needles, Set<Function> selected) {
        out.println("## Relevant strings and static references");
        out.println();
        Map<String, List<String>> rows = new LinkedHashMap<>();

        DataIterator definedData = currentProgram.getListing().getDefinedData(true);
        while (definedData.hasNext()) {
            Data data = definedData.next();
            StringDataInstance instance = StringDataInstance.getStringDataInstance(data);
            String value = instance == null ? null : instance.getStringValue();
            if (value == null || !matches(value, needles)) {
                continue;
            }

            List<String> references = new ArrayList<>();
            ReferenceIterator iterator = currentProgram.getReferenceManager().getReferencesTo(data.getAddress());
            while (iterator.hasNext()) {
                Reference reference = iterator.next();
                Function owner = currentProgram.getFunctionManager().getFunctionContaining(reference.getFromAddress());
                String ownerName = owner == null ? "<no function>" : owner.getName();
                references.add(reference.getFromAddress() + " in " + ownerName);
                if (owner != null) {
                    selected.add(owner);
                }
            }
            rows.put(data.getAddress() + " " + value, references);
        }

        for (Map.Entry<String, List<String>> row : rows.entrySet()) {
            out.println("- `" + row.getKey().replace("`", "'") + "`");
            for (String reference : row.getValue()) {
                out.println("  - referenced at `" + reference + "`");
            }
        }
        out.println();
    }

    private void writeDecompilations(Set<Function> selected, int maxFunctions) {
        out.println("## Selected function pseudocode");
        out.println();
        out.println("Decompiler output is reconstructed pseudocode, not original source.");
        out.println();

        DecompInterface decompiler = new DecompInterface();
        decompiler.toggleCCode(true);
        decompiler.toggleSyntaxTree(true);
        if (!decompiler.openProgram(currentProgram)) {
            out.println("Decompiler initialization failed: " + decompiler.getLastMessage());
            return;
        }

        int count = 0;
        try {
            for (Function function : selected) {
                if (count >= maxFunctions) {
                    out.println("Stopped after " + maxFunctions + " selected functions.");
                    break;
                }
                DecompileResults result = decompiler.decompileFunction(
                    function, 120, new ConsoleTaskMonitor());
                out.println("### `" + function.getName() + "` at `" +
                    function.getEntryPoint() + "`");
                out.println();
                if (!result.decompileCompleted() || result.getDecompiledFunction() == null) {
                    out.println("Decompilation failed: " + result.getErrorMessage());
                } else {
                    out.println("```c");
                    out.println(result.getDecompiledFunction().getC());
                    out.println("```");
                }
                out.println();
                count++;
            }
        } finally {
            decompiler.dispose();
        }
    }
}
