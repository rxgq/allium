using System.Text.Json;

sealed class AlliumApp {
  private AlliumCli? Cli;
  private readonly TerminalUtils Utils = new();
  private readonly JsonDb Db = new();

  public bool Run(string[] args) {
    var settings = Db.Get<WaterSettings>();
    if (settings is null) {
      return Setup();
    }

    if (args.Length is 0) {
        Utils.Println(Constants.AlliumInfo);
        return true;
    }

    settings = Db.Get<WaterSettings>();
    Cli = new(settings);

    return Cli.Execute(args);
  }

  private bool Setup() {
    Utils.Println("\nallium setup required..");

    var shouldSetup = Utils.Choice("continue?");
    if (!shouldSetup) {
      return false;
    }

    Utils.Println();

    string intakeUnit;
    for (;;) {
      intakeUnit = Utils.Read("enter your preferred measurement unit (ml, L, gal): ");
      if (intakeUnit.ToLower() is "ml" or "l" or "gal") {
        break;
      }
    }

    double intake;
    for (;;) {
      var unitStr = UnitMapper.Map(UnitMapper.Map(intakeUnit));
      var intakeStr = Utils.Read($"what is your daily intake goal in ({unitStr})?: ");

      if (double.TryParse(intakeStr, out intake)) {
        break;
      }
    }

    string name;
    for (;;) {
      name = Utils.Read("your name: ");

      if (name is not null && name?.Length >= 1) {
        break;
      }
    }

    var settings = new WaterSettings() {
        DailyGoal = intake,
        Unit = UnitMapper.Map(intakeUnit!),
        Username = name
    };

    Utils.Println("\ncreating allium directory");
    Directory.CreateDirectory(Constants.AppDataPath);
    Thread.Sleep(50);

    Utils.Println("creating water.json");
    using (File.Create(Constants.WaterPath)) { }
    File.WriteAllText(Constants.WaterPath, "[]");
    Thread.Sleep(30);
    
    Utils.Println("creating settings.json");
    using (File.Create(Constants.SettingsPath)) { }
    Thread.Sleep(30);

    Utils.Println("saving water settings");
    Thread.Sleep(200);
    Db.Write(settings);
    Utils.Println("saved settings");
    
    Utils.Println("finished setup.");
    return Utils.Info("\n  try using 'allium --log <amount>'");
  }
}