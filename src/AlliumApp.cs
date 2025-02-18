sealed class AlliumApp {
  private readonly AlliumCLI Cli = new();
  private readonly TerminalUtils Utils = new();
  private readonly JsonReader Reader = new();

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

    var tasks = Reader.Read<List<TaskModel>>(AlliumConstants.TasksPath);
    Utils.Print($"tasks {tasks.Count}");

    Utils.Print(" | ");

    var projects = Reader.Read<List<ProjectModel>>(AlliumConstants.ProjectsPath);
    Utils.Print($"projects {projects.Count}");
  }

  private void Login() {
    
  }
}