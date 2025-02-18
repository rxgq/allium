interface IReader {
  public T Read<T>(string path)
    where T : class;

  public void Write<T>(string path, T data)
    where T : class;
}