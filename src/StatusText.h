#pragma once

#include "KwlTypes.h"
#include "StageArbiter.h"

namespace Kwl
{
    /// Welche Fuehrung den hoechsten Stufenwunsch liefert.
    enum class Lead : uint8_t
    {
        None = 0,
        Humidity, ///< Feuchte-Treppe oder Stufe bei Entfeuchtung
        Co2,
        Voc,      ///< Luftguete
        Cooling,  ///< Temperaturfuehrung: freie Kuehlung
        WarmAir   ///< Temperaturfuehrung: Warmluft nutzen
    };

    /// 14 Zeichen DPT 16.001 und der Abschluss.
    constexpr uint8_t kStatusTextSize = 15;

    struct StatusInput
    {
        uint8_t stage;          ///< wirksame Stufe des Raums
        StageSource source;     ///< Rang, der sie bestimmt
        OperatingMode mode;     ///< aktive Betriebsart
        bool exhaust;           ///< Rang 3: Abluft (Bad) statt Zuluft (Nachstroemung)
        bool frost;             ///< Rang 2: Frostschutz statt Hitzeschutz
        Lead lead;              ///< staerkste Fuehrung
        uint8_t baseStage;      ///< Grundstufe der aktiven Betriebsart
    };

    // ========================================================================
    // Klartext-Status je Raum: "S<Stufe> <Grund>", hoechstens 14
    // Zeichen, ISO 8859-1. Der Grund folgt der Rangtabelle: Sperre, Schutz,
    // Bad/Zuluft, Verbund, Hand - und in der Automatik die Fuehrung, wenn die
    // WIRKSAME Stufe ueber der Grundstufe liegt, sonst die Betriebsart. Zaehlte
    // der Wunsch der Fuehrung vor dem Deckel, hiesse es nachts mit
    // Maximalstufe 1 "S1 Feuchte" und in der Intervallpause "S0 CO2".
    // ========================================================================
    void statusText(const StatusInput& in, char out[kStatusTextSize]);
} // namespace Kwl
