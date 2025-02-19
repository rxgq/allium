using System.Security.Cryptography.X509Certificates;
using System.Text.Json;

sealed class JsonReader : IReader {
  public void Write<T>(string path, T data)
    where T : class
  {
    var content = Read<List<T>>(path);
    content.Add(data);

    var jsonContent = JsonSerializer.Serialize(content);
    File.WriteAllText(path, jsonContent);
  }

  public T Read<T>(string path)
    where T : class
  {
    var content = File.ReadAllText(path);
    var data = JsonSerializer.Deserialize<T>(content);

    return data;
  }

  public void Update<T>(string path, T data) 
    where T : class
  {
    var listData = Read<List<T>>(path);
    var item = listData.Where(x => x.Equals(data)).First();

    listData.Remove(item);
    listData.Add(data);

    WriteList(path, listData);
  }

  public void Delete<T>(string path, T data) 
    where T : class 
  {
    var listData = Read<List<T>>(path);
    var item = listData.Where(x => x.Equals(data)).First();
    
    listData.Remove(item);

    WriteList(path, listData);
  }

  private static void WriteList<T>(string path, List<T> items) 
    where T : class 
  {
    var jsonContent = JsonSerializer.Serialize(items);
    File.WriteAllText(path, jsonContent);
  }
}