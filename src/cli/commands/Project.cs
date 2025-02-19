sealed partial class AlliumCLI {
      private bool ProjectHandler() {
        if (Args.Count < 1) return ProjectHelp();

        var action = Args[0];
        return action switch {
            "new"  => NewProject(),
            "list" => ListProjects(),
            "-h" or "--help" or _ => ProjectHelp(),
        };
    }

    private bool NewProject() {
        if (Args.Count < 2) return ExpectArg("project name");

        var projects = JsonController.Read<List<ProjectModel>>();
        var nextId = projects.Count != 0 ? projects.Max(x => x.Id) + 1 : 0;
        
        var name = string.Join(' ', Args.Skip(1).ToList());

        var project = new ProjectModel() {
            Name = name,
            Id = nextId
        };
        
        JsonController.Create(project);
        return Utils.Info($"project '{name}' added.");
    }

    private bool ListProjects() {
        var projects = JsonController.Read<List<ProjectModel>>() ?? [];

        if (projects.Count == 0) {
            return Utils.Info("no projects available.");
        }
        
        var task = Utils.MenuList(projects, $"projects ({projects.Count})", (x) => x.Name);
        return true;
    }

    private bool ProjectHelp() {
        Utils.PrintLn(AlliumConstants.ProjectHelp);
        return true;
    }
}