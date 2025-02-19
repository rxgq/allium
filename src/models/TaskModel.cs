sealed class TaskModel {
  public int Id { get; set; }
  public string Name { get; set; }
  public string Description { get; set; }
  public bool IsComplete { get; set; }

  public override bool Equals(object obj) {
      if (obj is TaskModel otherTask) {
        return Id == otherTask.Id;
      }
      
      return false;
  }
}