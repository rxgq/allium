sealed class AlliumApp {
  private readonly AlliumCLI Cli = new();
  private readonly TerminalUtils Utils = new();
  private readonly JsonController JsonController = new();

  private AlliumUser user;
  private string? Input;

  public void Run() {
    user = new() { Name = "ashton" };

    Console.Clear();
    Utils.Info("\n  welcome, sir.\n");

    for (;;) {
      if (Input is not "") {
        WriteHome();
        Utils.PrintLn("\n");
      }

      Input = Utils.Read($"@{user.Name} > ");
      if (Input.Trim().ToLower() is "q") {
        break;
      }

      Cli.Execute(Input);

      if (Input is not "" and not "cls") {
        Console.ReadKey();
        Console.Clear();
      }
    }

    Utils.PrintLn("\n  goodbye, friend.\n");
  }

  private void WriteHome() {
    Utils.Print("  ");

    var tasks = JsonController.Read<List<TaskModel>>();
    Utils.Print($"tasks {tasks.Count}");

    Utils.Print(" | ");

    var projects = JsonController.Read<List<ProjectModel>>();
    Utils.Print($"projects {projects.Count}");
  }
}