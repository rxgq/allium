using System.Data;

sealed class JsonController {
    private readonly JsonReader Reader = new();

    public T Read<T>() where T : class {
        return Reader.Read<T>(MapPath<T>());
    }

    public void Create<T>(T data) where T : class {
        Reader.Write(MapPath<T>(), data);
    }

    public void Update<T>(T data) where T : class {
        Reader.Update(MapPath<T>(), data);
    }

    public void Delete<T>(T data) where T : class {
        Reader.Delete(MapPath<T>(), data);
    }

    private string MapPath<T>() where T : class {
        return typeof(T) switch {
            Type t when t == typeof(TaskModel) || t == typeof(List<TaskModel>) => AlliumConstants.TasksPath,
            Type t when t == typeof(ProjectModel) || t == typeof(List<ProjectModel>) => AlliumConstants.ProjectsPath,
            Type t when t == typeof(MacroModel) || t == typeof(List<MacroModel>) => AlliumConstants.MacroPath,
            _ => throw new InvalidOperationException($"Unsupported type: {typeof(T).Name}")
        };
    }
}
