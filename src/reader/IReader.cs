interface IReader {
  public void Write<T>(string path, T data)
    where T : class;
    
  public T Read<T>(string path)
    where T : class;

  public void Update<T>(string path, T data)
    where T : class;

  public void Delete<T>(string path, T data)
    where T : class;
}