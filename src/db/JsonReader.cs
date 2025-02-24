using System.Text.Json;

sealed class JsonDb {
    public List<T> Get<T>() {
        var path = MapPath<T>();

        var json = File.ReadAllText(path);
        return JsonSerializer.Deserialize<List<T>>(json);
    }

    public void Write<T>(T data) {
        var path = MapPath<T>();

        var existingData = Get<T>();
        existingData.Add(data);

        var json = JsonSerializer.Serialize(existingData);
        File.WriteAllText(path, json);
    }

    public void Write<T>(List<T> data, bool overwrite = false) {
        var path = MapPath<T>();

        var existingData = Get<T>();
        existingData.AddRange(data);

        var json = JsonSerializer.Serialize(overwrite ? data : existingData);
        File.WriteAllText(path, json);
    }

    private static string MapPath<T>() {
        return typeof(T) switch {
            var t when t == typeof(WaterEntry) => Constants.WaterPath,
            _ => ""
        };
    }
}