using System.Text.Json;

sealed class JsonDb {
    private static readonly JsonSerializerOptions JsonOptions = new() {
        WriteIndented = true
    };

    public List<T> GetList<T>() {
        try {
            var path = MapPath<T>();

            var json = File.ReadAllText(path);
            return JsonSerializer.Deserialize<List<T>>(json);
        }
        catch {
            return default;
        }
    }

    public T Get<T>() {
        try {
            var path = MapPath<T>();

            var json = File.ReadAllText(path);
            return JsonSerializer.Deserialize<T>(json);
        }
        catch {
            return default;
        }
    }

    public void Write<T>(T data) {
        var path = MapPath<T>();

        var json = JsonSerializer.Serialize(data, JsonOptions);
        File.WriteAllText(path, json);
    }

    public void Write<T>(List<T> data, bool overwrite = false) {
        var path = MapPath<T>();

        var existingData = GetList<T>();
        existingData.AddRange(data);

        var json = JsonSerializer.Serialize(overwrite ? data : existingData, JsonOptions);
        File.WriteAllText(path, json);
    }

    private static string MapPath<T>() {
        return typeof(T) switch {
            var t when t == typeof(WaterEntry) => Constants.WaterPath,
            var t when t == typeof(WaterSettings) => Constants.SettingsPath,
            _ => ""
        };
    }
}