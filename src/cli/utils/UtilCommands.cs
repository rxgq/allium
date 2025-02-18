sealed partial class AlliumCLI {
    private bool UtilsHandler() {
        return Command switch {
            "echo" => Echo(Args),
            "cls"  => Clear(Args),
            _ => Utils.Error($"unknown command '{Command}'")
        };
    }

    private bool Echo(List<string> args) {
        if (args.Count < 1) return Expect("one argument");

        var message = string.Join(' ', args.ToList());
        return Utils.Info(message);
    }

    private bool Clear(List<string> args) {
        Console.Clear();
        return true;
    }
}