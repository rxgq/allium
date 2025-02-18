using System.Text.Json;

sealed class JsonReader : IReader {
  public T Read<T>(string path)
    where T : class
  {
    var content = File.ReadAllText(path);
    var data = JsonSerializer.Deserialize<T>(content);

    return data;
  }

  public void Write<T>(string path, T data)
    where T : class
  {
    var content = Read<List<T>>(path);
    content.Add(data);

    var jsonContent = JsonSerializer.Serialize(content);
    File.WriteAllText(path, jsonContent);
  }
}