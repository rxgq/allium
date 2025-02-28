
static class UnitMapper {
    public static string Map(WaterUnit unit) {
        return unit switch {
            WaterUnit.gallon => "gal",
            WaterUnit.litre => "L",
            WaterUnit.milliliter => "ml",
            _ => ""
        }; 
    }

    public static WaterUnit Map(string str) {
        return str.ToLower() switch {
            "gal" => WaterUnit.gallon,
            "l" => WaterUnit.litre,
            "ml" => WaterUnit.milliliter,
            _ => WaterUnit.Unknown
        }; 
    }
}