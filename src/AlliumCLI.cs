sealed class AlliumCli {
    private readonly Dictionary<string, Func<List<string>, bool>> Commands;
    private readonly TerminalUtils Utils = new();

    public AlliumCli() {
        Commands = new() {
            ["--help"] = ShowHelp,
            ["-h"] = ShowHelp,
            ["--version"] = ShowVersion,
            ["-v"] = ShowVersion,
            ["--log"] = LogWater,
            ["--list"] = ListEntries,
        };
    }

    public bool Execute(string[] args) {
        var command = args[0];

        if (Commands.TryGetValue(command, out var value)) {
            return value([.. args.Skip(1)]);
        }

        return true;
    }

    private bool ShowHelp(List<string> args) {
        Utils.Println(Constants.AlliumInfo);
        return true;
    }

    private bool ShowVersion(List<string> args) {
        Utils.Println(Constants.AlliumVersion);
        return true;
    }

    private bool LogWater(List<string> args) {

        return true;
    }

    private bool ListEntries(List<string> args) {

        return true;
    }
}