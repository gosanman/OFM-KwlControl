#include "StatusText.h"

#include <cstdio>

namespace Kwl
{
    namespace
    {
        // ISO 8859-1, wie DPT 16.001 es verlangt: ue = \xFC, oe = \xF6, ss = \xDF.
        const char* modeText(OperatingMode mode)
        {
            switch (mode)
            {
                case OperatingMode::Comfort:
                    return "Komfort";
                case OperatingMode::Standby:
                    return "Standby";
                case OperatingMode::Eco:
                    return "Nacht";
                case OperatingMode::Protection:
                    return "Schutz";
                case OperatingMode::Boost:
                    return "Sto\xDFl\xFC" "ften";
                case OperatingMode::Reduction:
                    return "Absenkung";
                case OperatingMode::Quiet:
                    return "Ruhe";
                case OperatingMode::Auto:
                default:
                    return "Standard";
            }
        }

        const char* leadText(Lead lead)
        {
            switch (lead)
            {
                case Lead::Humidity:
                    return "Feuchte";
                case Lead::Co2:
                    return "CO2";
                case Lead::Voc:
                    return "Luftg\xFCte";
                case Lead::Cooling:
                    return "K\xFChlen";
                case Lead::WarmAir:
                    return "Warmluft";
                case Lead::None:
                default:
                    return "";
            }
        }

        const char* reasonText(const StatusInput& in)
        {
            switch (in.source)
            {
                case StageSource::Lock:
                    return "Sperre";
                case StageSource::Protection:
                    return in.frost ? "Frostschutz" : "Hitzeschutz";
                case StageSource::AirDemand:
                    return in.exhaust ? "Bad" : "Zuluft";
                case StageSource::Group:
                    return "Verbund";
                case StageSource::Manual:
                    return "Hand";
                case StageSource::Automatic:
                default:
                    // Die Fuehrung nennt sich nur, wenn die wirksame Stufe ueber
                    // der Grundstufe liegt - sonst haette die Betriebsart dieselbe
                    // Stufe geliefert (Deckel, Intervallpause, Stillstand bei
                    // fehlenden Messwerten).
                    if (in.lead != Lead::None && in.stage > in.baseStage)
                        return leadText(in.lead);
                    return modeText(in.mode);
            }
        }
    } // namespace

    void statusText(const StatusInput& in, char out[kStatusTextSize])
    {
        const unsigned stage = in.stage > kStageMax ? kStageMax : in.stage;
        snprintf(out, kStatusTextSize, "S%u %s", stage, reasonText(in));
    }
} // namespace Kwl
