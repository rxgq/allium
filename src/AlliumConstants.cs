static class AlliumConstants {
  public static readonly string TaskHelp = @"
usage: task [command] [options]

commands:
  new   <name>  create a new task with the given name.
  list          list all existing tasks.
  -h, --help    show this help message.
";

  public static readonly string ProjectHelp = @"
usage: project [command] [options]

commands:
  new <name>  create a new project with the given name.
  list        list all existing projects.
  -h, --help  show this help message.
";

  public static readonly string TasksPath = "data/tasks.json";
  public static readonly string ProjectsPath = "data/projects.json";
  public static readonly string MacroPath = "data/macros.json";
  public static string UserProjectsPath = "C:\\Users\\rxgqq\\projects";

  public static readonly string AppDataPath = 
    Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), "allium");

  public static readonly string UserPath = Path.Combine(AppDataPath, "user.json");
}