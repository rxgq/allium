sealed partial class AlliumCLI {
    private bool Expect(string arg) {
        return Utils.Error($"command '{Command}' expects {arg}.");
    }

    private bool ExpectArg(string arg) {
        return Utils.Error($"command '{Command} {Args.First()}' expects {arg}.");
    }
}
