sealed class AlliumCli {
    private readonly Dictionary<string, Func<List<string>, bool>> Commands;
    private readonly TerminalUtils Utils = new();
    private readonly JsonDb Db = new();
    private readonly WaterSettings Settings;

    public AlliumCli(WaterSettings settings) {
        Commands = new() {
            ["--help"] = ShowHelp,
            ["-h"] = ShowHelp,
            ["--version"] = ShowVersion,
            ["-v"] = ShowVersion,
            ["--log"] = LogWater,
            ["--list"] = ListEntries,
            ["--undo"] = UndoEntry,
            ["--setting"] = GetSetting
        };

        Settings = settings;
    }

    public bool Execute(string[] args) {
        var command = args[0];

        if (Commands.TryGetValue(command, out var value)) {
            return value(args.ToList());
        }

        return true;
    }

    private bool GetSetting(List<string> args) {
        var settingMap = new Dictionary<string, object>() {
            ["goal"] = Settings.DailyGoal,
            ["user"] = Settings.Username,
            ["unit"] = Settings.Unit
        };


        if (args.Count == 1) {
            Utils.Error("expected specified setting after '--setting'.");
            return false;
        }

        var setting = args[1];
        if (settingMap.TryGetValue(setting, out var settingValue)) {
            Utils.Println(settingValue.ToString());
        } else {
            return Utils.Error($"unknown setting '{setting}'");
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
        if (args.Count == 1) {
            Utils.Error("expected specified amount after '--log'.");
            return false;
        }

        var amount = args[1];
        if (!double.TryParse(amount, out _)) {
            Utils.Error($"invalid water amount '{amount}'.");
            return false;
        }

        var entry = new WaterEntry() {
            WaterAmount = double.Parse(amount),
            DateEntered = DateTime.Now,
            Unit = WaterUnit.milliliter
        };

        var entries = Db.GetList<WaterEntry>()
            .Append(entry).ToList();

        Db.Write(entries, overwrite: true);

        return Utils.Info("water entry logged.");
    }

    private bool ListEntries(List<string> args) {
        var entries = Db.GetList<WaterEntry>();
        
        if (entries.Count == 0) {
            Utils.Error("no water entries found.");
            return false;
        }

        foreach (var entry in entries) {
            var unit = UnitMapper.Map(entry.Unit);
            Utils.Println($"{entry.DateEntered}: {entry.WaterAmount}{unit}");
        }

        return true;
    }

    private bool UndoEntry(List<string> args) {
        var entries = Db.GetList<WaterEntry>();
        if (entries.Count == 0) {
            Utils.Error("no water entries found.");
            return false;
        }

        entries.RemoveAt(entries.Count - 1);
        Db.Write(entries, overwrite: true);

        return Utils.Info("last water entry removed.");
    }
}