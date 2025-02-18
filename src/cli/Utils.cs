sealed partial class AlliumCLI {
    private bool Expect(string arg) {
        return Utils.Error($"command '{currentCommand}' expects {arg}.");
    }
}
