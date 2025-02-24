
static class UnitMapper {
    public static string Map(WaterUnit unit) {
        return unit switch {
            WaterUnit.gallon => "gal",
            WaterUnit.litre => "L",
            WaterUnit.millilitre => "ml",
            _ => ""
        }; 
    }
}