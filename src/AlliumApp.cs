sealed class AlliumApp {
    private readonly AlliumCli Cli = new();
    private readonly TerminalUtils Utils = new();

    public void Run(string[] args) {
        if (args.Length is 0) {
            Utils.Println(Constants.AlliumInfo);
        }

        Cli.Execute(args);
    }
}