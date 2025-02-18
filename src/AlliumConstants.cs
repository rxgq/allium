static class AlliumConstants {
  public static string TaskHelp = @"
usage: task [command] [options]

commands:
  new   <name>  create a new task with the given name.
  list          list all existing tasks.
  -h, --help    show this help message.
";

  public static string ProjectHelp = @"
usage: project [command] [options]

commands:
  new <name>  create a new project with the given name.
  list        list all existing projects.
  -h, --help  show this help message.
";

  public static string TasksPath = "data/tasks.json";
  public static string ProjectsPath = "data/projects.json";
  public static string MacroPath = "data/macros.json";

}