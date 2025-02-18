sealed partial class AlliumCLI {
    private bool UtilsHandler(List<string> args) {
        var command = args[0];

        return command switch {
            "echo" => Echo(args),
            "cls"  => Clear(args),
            _ => Utils.Error($"unknown command '{command}'")
        };
    }

    private bool Echo(List<string> args) {
        if (args.Count < 2) return Expect("one argument");

        var message = string.Join(' ', args.Skip(1).ToList());
        return Utils.Info(message);
    }

    private bool Clear(List<string> args) {
        Console.Clear();
        return true;
    }
}