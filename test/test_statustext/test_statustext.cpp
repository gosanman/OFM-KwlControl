// Tests des Klartext-Status je Raum.
//
// Der Text sagt in hoechstens 14 Zeichen (DPT 16.001), welche Stufe gerade
// laeuft und warum - in der Reihenfolge der Rangtabelle (Invariante 9). Umlaute
// sind ISO 8859-1, so wie DPT 16.001 sie verlangt.

#include <unity.h>

#include <cstring>

#include "StatusText.h"

using namespace Kwl;

void setUp(void) {}
void tearDown(void) {}

namespace
{
    StatusInput base(uint8_t stage, StageSource source)
    {
        StatusInput in;
        in.stage = stage;
        in.source = source;
        in.mode = OperatingMode::Comfort;
        in.exhaust = true;
        in.frost = false;
        in.lead = Lead::None;
        in.baseStage = 2;
        return in;
    }

    void expect(const char* want, const StatusInput& in)
    {
        char out[kStatusTextSize];
        statusText(in, out);
        TEST_ASSERT_EQUAL_STRING(want, out);
    }
} // namespace

static void test_sperre(void)
{
    expect("S0 Sperre", base(0, StageSource::Lock));
    // Verhalten bei Sperre "Grundstufe": die Stufe steht trotzdem davor.
    expect("S1 Sperre", base(1, StageSource::Lock));
}

static void test_frost_und_hitzeschutz(void)
{
    StatusInput in = base(0, StageSource::Protection);
    in.frost = true;
    expect("S0 Frostschutz", in);
    in = base(1, StageSource::Protection);
    in.frost = false;
    expect("S1 Hitzeschutz", in);
}

static void test_bad_und_nachstroemung(void)
{
    StatusInput in = base(4, StageSource::AirDemand);
    in.exhaust = true;
    expect("S4 Bad", in);
    in.exhaust = false;
    in.stage = 2;
    expect("S2 Zuluft", in);
}

static void test_verbund_und_hand(void)
{
    expect("S2 Verbund", base(2, StageSource::Group));
    expect("S3 Hand", base(3, StageSource::Manual));
}

static void test_automatik_nennt_die_fuehrung(void)
{
    StatusInput in = base(3, StageSource::Automatic);
    in.lead = Lead::Humidity;
    expect("S3 Feuchte", in);
    in.lead = Lead::Co2;
    expect("S3 CO2", in);
    in.lead = Lead::Voc;
    expect("S3 Luftg\xFCte", in);
    in.lead = Lead::Cooling;
    expect("S3 K\xFChlen", in);
    in.lead = Lead::WarmAir;
    expect("S3 Warmluft", in);
}

static void test_automatik_gedeckelt_nennt_trotzdem_die_fuehrung(void)
{
    // CO2 will 4, die Maximalstufe deckelt auf 2: die Stufe kommt trotzdem
    // vom CO2, nicht von der Grundstufe.
    StatusInput in = base(2, StageSource::Automatic);
    in.baseStage = 1;
    in.lead = Lead::Co2;
    expect("S2 CO2", in);
}

static void test_automatik_auf_grundstufe_nennt_die_betriebsart(void)
{
    // Fuehrung unter oder gleich der Grundstufe: die Betriebsart bestimmt.
    StatusInput in = base(2, StageSource::Automatic);
    in.lead = Lead::Humidity;
    expect("S2 Komfort", in);
    in.lead = Lead::None;
    expect("S2 Komfort", in);

    in.stage = 1;
    in.baseStage = 1;
    in.mode = OperatingMode::Standby;
    expect("S1 Standby", in);
    in.mode = OperatingMode::Eco;
    expect("S1 Nacht", in);
    in.mode = OperatingMode::Protection;
    expect("S1 Schutz", in);
    in.mode = OperatingMode::Reduction;
    expect("S1 Absenkung", in);
    in.stage = 4;
    in.mode = OperatingMode::Boost;
    expect("S4 Sto\xDFl\xFC" "ften", in);
    in.stage = 0;
    in.mode = OperatingMode::Quiet;
    expect("S0 Ruhe", in);
}

static void test_hoechstens_vierzehn_zeichen(void)
{
    // Alle Texte passen in DPT 16.001 (14 Zeichen + Abschluss).
    const StageSource sources[] = {StageSource::Lock, StageSource::Protection, StageSource::AirDemand,
                                   StageSource::Group, StageSource::Manual, StageSource::Automatic};
    const OperatingMode modes[] = {OperatingMode::Auto, OperatingMode::Comfort, OperatingMode::Standby,
                                   OperatingMode::Eco, OperatingMode::Protection, OperatingMode::Boost,
                                   OperatingMode::Reduction, OperatingMode::Quiet};
    const Lead leads[] = {Lead::None, Lead::Humidity, Lead::Co2, Lead::Voc, Lead::Cooling, Lead::WarmAir};
    char out[kStatusTextSize];
    for (StageSource s : sources)
        for (OperatingMode m : modes)
            for (Lead l : leads)
                for (int f = 0; f < 2; f++)
                {
                    StatusInput in = base(4, s);
                    in.mode = m;
                    in.lead = l;
                    in.frost = f == 1;
                    statusText(in, out);
                    TEST_ASSERT_TRUE(strlen(out) <= 14);
                    TEST_ASSERT_TRUE(strlen(out) >= 4);
                }
}

static void test_stufe_wird_geklemmt(void)
{
    // Ein Ueberlauf darf keinen langen Text erzeugen.
    expect("S4 Hand", base(200, StageSource::Manual));
}

static void test_automatik_deckel_und_pause_nennen_die_betriebsart(void)
{
    // Nachts deckelt die Maximalstufe 1 den Feuchtewunsch 3: die wirksame
    // Stufe ist die Grundstufe, also "Nacht", nicht "Feuchte". In der
    // Intervallpause steht der Raum trotz CO2-Wunsch: "Komfort".
    StatusInput in = base(1, StageSource::Automatic);
    in.baseStage = 1;
    in.mode = OperatingMode::Eco;
    in.lead = Lead::Humidity;
    expect("S1 Nacht", in);

    in.stage = 0;
    in.mode = OperatingMode::Comfort;
    in.lead = Lead::Co2;
    expect("S0 Komfort", in);
}

int main(int, char**)
{
    UNITY_BEGIN();
    RUN_TEST(test_sperre);
    RUN_TEST(test_frost_und_hitzeschutz);
    RUN_TEST(test_bad_und_nachstroemung);
    RUN_TEST(test_verbund_und_hand);
    RUN_TEST(test_automatik_nennt_die_fuehrung);
    RUN_TEST(test_automatik_gedeckelt_nennt_trotzdem_die_fuehrung);
    RUN_TEST(test_automatik_auf_grundstufe_nennt_die_betriebsart);
    RUN_TEST(test_hoechstens_vierzehn_zeichen);
    RUN_TEST(test_stufe_wird_geklemmt);
    RUN_TEST(test_automatik_deckel_und_pause_nennen_die_betriebsart);
    return UNITY_END();
}
