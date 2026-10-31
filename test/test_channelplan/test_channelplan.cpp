// Tests der Kanalbelegungspruefung.
//
// Die ETS laesst den DAC-Kanal je Luefter frei waehlen. Was sie nicht prueft,
// prueft die Firmware beim Start: Kanal auf der Platine, ego auf dem ersten
// Kanal eines Chips, kein Kanal mit zwei Lueftern. Jeder Fall ergibt Fehlercode
// 3 am betroffenen Luefter - und bei einer Ueberschneidung an BEIDEN.

#include <unity.h>

#include "ChannelPlan.h"

using namespace Kwl;

void setUp(void) {}
void tearDown(void) {}

static void test_gueltige_belegung_bleibt_frei(void)
{
    // ego auf S1/S2, e2 60 auf S3 und S4: Platine Rev 0.1 voll belegt.
    const ChannelUse use[3] = {{true, 0, 2}, {true, 2, 1}, {true, 3, 1}};
    ChannelFault f[3];
    TEST_ASSERT_EQUAL_UINT8(0, checkChannelPlan(use, 3, 4, f));
    for (uint8_t i = 0; i < 3; i++)
        TEST_ASSERT_EQUAL(ChannelFault::None, f[i]);
}

static void test_kanal_jenseits_der_platine(void)
{
    // Kanal 5 (ETS) auf der 4-Kanal-Platine, und ein ego, dessen zweiter Motor
    // hinter der Platine laege.
    const ChannelUse use[2] = {{true, 4, 1}, {true, 3, 2}};
    ChannelFault f[2];
    TEST_ASSERT_EQUAL_UINT8(2, checkChannelPlan(use, 2, 4, f));
    TEST_ASSERT_EQUAL(ChannelFault::OutOfRange, f[0]);
    TEST_ASSERT_EQUAL(ChannelFault::OutOfRange, f[1]);
}

static void test_ego_auf_zweitem_kanal_eines_chips(void)
{
    const ChannelUse use[1] = {{true, 1, 2}};
    ChannelFault f[1];
    TEST_ASSERT_EQUAL_UINT8(1, checkChannelPlan(use, 1, 4, f));
    TEST_ASSERT_EQUAL(ChannelFault::PairStart, f[0]);
}

static void test_doppelbelegung_markiert_beide(void)
{
    // Luefter 1 und 2 beide auf S1; Luefter 3 ordentlich auf S2.
    const ChannelUse use[3] = {{true, 0, 1}, {true, 0, 1}, {true, 1, 1}};
    ChannelFault f[3];
    TEST_ASSERT_EQUAL_UINT8(2, checkChannelPlan(use, 3, 4, f));
    TEST_ASSERT_EQUAL(ChannelFault::Collision, f[0]);
    TEST_ASSERT_EQUAL(ChannelFault::Collision, f[1]);
    TEST_ASSERT_EQUAL(ChannelFault::None, f[2]);
}

static void test_ego_ueberschneidet_sich_mit_einzelkanal(void)
{
    // ego auf S1/S2, e2 60 auf S2: der zweite Motor und der e2 60 teilen sich S2.
    const ChannelUse use[2] = {{true, 0, 2}, {true, 1, 1}};
    ChannelFault f[2];
    TEST_ASSERT_EQUAL_UINT8(2, checkChannelPlan(use, 2, 4, f));
    TEST_ASSERT_EQUAL(ChannelFault::Collision, f[0]);
    TEST_ASSERT_EQUAL(ChannelFault::Collision, f[1]);
}

static void test_drei_luefter_auf_einem_kanal(void)
{
    const ChannelUse use[3] = {{true, 2, 1}, {true, 2, 1}, {true, 2, 1}};
    ChannelFault f[3];
    TEST_ASSERT_EQUAL_UINT8(3, checkChannelPlan(use, 3, 4, f));
    for (uint8_t i = 0; i < 3; i++)
        TEST_ASSERT_EQUAL(ChannelFault::Collision, f[i]);
}

static void test_inaktive_luefter_belegen_nichts(void)
{
    // Luefter 1 inaktiv auf S1, Luefter 2 aktiv auf S1: kein Konflikt.
    const ChannelUse use[2] = {{false, 0, 1}, {true, 0, 1}};
    ChannelFault f[2];
    TEST_ASSERT_EQUAL_UINT8(0, checkChannelPlan(use, 2, 4, f));
    TEST_ASSERT_EQUAL(ChannelFault::None, f[0]);
    TEST_ASSERT_EQUAL(ChannelFault::None, f[1]);
}

static void test_zwoelf_kanaele_auf_grosser_platine(void)
{
    ChannelUse use[12];
    ChannelFault f[12];
    for (uint8_t i = 0; i < 12; i++)
        use[i] = {true, i, 1};
    TEST_ASSERT_EQUAL_UINT8(0, checkChannelPlan(use, 12, 12, f));
    // Dieselbe Belegung auf der 4-Kanal-Platine: acht Luefter liegen daneben.
    TEST_ASSERT_EQUAL_UINT8(8, checkChannelPlan(use, 12, 4, f));
    TEST_ASSERT_EQUAL(ChannelFault::None, f[3]);
    TEST_ASSERT_EQUAL(ChannelFault::OutOfRange, f[4]);
}

int main(int, char**)
{
    UNITY_BEGIN();
    RUN_TEST(test_gueltige_belegung_bleibt_frei);
    RUN_TEST(test_kanal_jenseits_der_platine);
    RUN_TEST(test_ego_auf_zweitem_kanal_eines_chips);
    RUN_TEST(test_doppelbelegung_markiert_beide);
    RUN_TEST(test_ego_ueberschneidet_sich_mit_einzelkanal);
    RUN_TEST(test_drei_luefter_auf_einem_kanal);
    RUN_TEST(test_inaktive_luefter_belegen_nichts);
    RUN_TEST(test_zwoelf_kanaele_auf_grosser_platine);
    return UNITY_END();
}
