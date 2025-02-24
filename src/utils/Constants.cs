static class Constants
{
#if DEBUG
  public static readonly string AppDataPath = "data/";
#else
  public static readonly string AppDataPath = Path.Combine(
      Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData),
      "allium"
  );
#endif

  public static readonly string WaterPath = Path.Combine(AppDataPath, "water.json");
  public static readonly string SettingsPath = Path.Combine(AppDataPath, "settings.json");

  public static readonly string AlliumInfo =
@"
usage:
  allium <command> [options]

commands:
  -v, --version            Show version
";

  public static readonly string AlliumVersion = "allium 0.0.1";
}
