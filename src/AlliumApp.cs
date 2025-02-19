using System.Text.Json;

sealed class AlliumApp {
  private readonly AlliumCLI Cli = new();
  private readonly TerminalUtils Utils = new();
  private readonly JsonController JsonController = new();

  private AlliumUser? User;
  private string? Input;

  public void Run() {
    Console.Clear();

    // Reset();
    if (!Directory.Exists(AlliumConstants.AppDataPath)) {
      Setup();
    }      

    Utils.Info("  allium\n");

    var isAllowed = Authenticate();
    if (!isAllowed) return;

    Console.Clear();
    Utils.Info("\n  welcome, sir.\n");

    for (;;) {
      if (Input is not "") {
        WriteHome();
      }

      Input = Utils.Read($"@{User!.Name} > ");
      if (Input.Trim().ToLower() is "q") {
        break;
      }

      Cli.Execute(Input);

      if (Input is not "" and not "cls") {
        Console.Clear();
      }
    }

    Utils.PrintLn("\n  goodbye, friend.\n");
  }

  private bool Authenticate() {
    var userJson = File.ReadAllText(AlliumConstants.UserPath);
    User = JsonSerializer.Deserialize<AlliumUser>(userJson);

    if (User is null) {
      return false;
    }

    if (User.Pin is null) {
      return true;
    }

    for (;;) {
      var pin = Utils.Read("pin: ");
      if (pin == User?.Pin) {
        return true;
      }
    }
  }

  private void Setup() {
    Utils.PrintLn("allium setup required");

    var user = new AlliumUser {
        Name = Utils.Read("\n  name: ")
    };

    Utils.PrintLn();
    var setupPin = Utils.Choice("setup authentication?");
    string? pin;
    if (setupPin) {
      for (;;) {
        pin = Utils.Read("\n  pin: ");
        var confirmPin = Utils.Read("  confirm: ");

        if (pin != confirmPin) {
          Utils.PrintLn("pins were not equal.");
          continue;
        }

        user.Pin = pin;
        break;
      }
    }

    User = user;

    Directory.CreateDirectory(AlliumConstants.AppDataPath);
    using (File.Create(AlliumConstants.UserPath)) { }

    var userJson = JsonSerializer.Serialize(User);
    File.WriteAllText(AlliumConstants.UserPath, userJson);
  }

  private void WriteHome() {
    Utils.Print("  ");

    var tasks = JsonController.Read<List<TaskModel>>();
    Utils.Print($"tasks {tasks.Count}");

    Utils.Print(" | ");

    var projects = JsonController.Read<List<ProjectModel>>();
    Utils.Print($"projects {projects.Count}");

    Utils.PrintLn("\n");
  }
}