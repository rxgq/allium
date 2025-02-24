using System.Text.Json;

sealed class JsonReader {
    public static List<T> Read<T>() {
        var path = MapPath<T>();

        var json = File.ReadAllText(path);
        return JsonSerializer.Deserialize<List<T>>(json);
    }

    public static void Write<T>(T data) {
        var path = MapPath<T>();

        var existingData = Read<T>();
        existingData.Add(data);

        var json = JsonSerializer.Serialize(existingData);
        File.WriteAllText(path, json);
    }

    private static string MapPath<T>() {
        return typeof(T) switch {
            var t when t == typeof(WaterEntry) => Constants.WaterPath,
            _ => ""
        };
    }
}