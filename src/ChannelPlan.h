#pragma once

#include <stdint.h>

namespace Kwl
{
    /// Kanalbelegung eines Luefters, wie sie die Pruefung beim Start braucht.
    struct ChannelUse
    {
        bool active;   ///< nur aktive Luefter belegen Kanaele
        uint8_t first; ///< erster DAC-Kanal, ab 0
        uint8_t count; ///< 1, beim ego 2
    };

    enum class ChannelFault : uint8_t
    {
        None = 0,
        OutOfRange, ///< Kanal liegt nicht auf der Platine
        PairStart,  ///< ego beginnt nicht auf dem ersten Kanal eines Chips
        Collision   ///< Kanal gehoert auch einem anderen Luefter
    };

    /// Kanalbelegung aller Luefter gegen die Platine pruefen.
    ///
    /// Drei Fehler, die die ETS nicht abfaengt, weil der Kanal je Luefter
    /// frei waehlbar ist (1…12, unabhaengig von der Platine):
    ///  - ein Kanal jenseits der Platine: der Luefter liefe "virtuell", ohne
    ///    Stellsignal und ohne diese Pruefung ohne Fehlercode (Invariante 10);
    ///  - ein ego auf dem zweiten Kanal eines Chips: beide Motoren laegen auf
    ///    zwei Chips und liessen sich nicht in einem Frame stellen (Invariante 6);
    ///  - ein Kanal mit zwei Lueftern: beide schrieben abwechselnd ihre Spannung,
    ///    mit entgegengesetzter Phase im Takt ein Richtungswechsel in jedem
    ///    Durchlauf - am Totzeitwaechter vorbei, der nur die eigene Geschichte
    ///    kennt (Invariante 5).
    ///
    /// Bei einer Ueberschneidung werden BEIDE Luefter markiert: welcher von
    /// beiden "zuerst" parametriert wurde, ist keine Entscheidungsgrundlage.
    ///
    /// @param use           Belegung je Luefter, `n` Eintraege
    /// @param boardChannels Kanaele der Platine (FANDRV_BOARD_CHANNELS)
    /// @param fault         Ergebnis je Luefter, `n` Eintraege
    /// @return Zahl der markierten Luefter
    inline uint8_t checkChannelPlan(const ChannelUse* use, uint8_t n, uint8_t boardChannels,
                                    ChannelFault* fault)
    {
        constexpr uint8_t kChannelsMax = 16;
        int8_t owner[kChannelsMax];
        for (uint8_t c = 0; c < kChannelsMax; c++)
            owner[c] = -1;
        if (boardChannels > kChannelsMax)
            boardChannels = kChannelsMax;

        for (uint8_t i = 0; i < n; i++)
        {
            fault[i] = ChannelFault::None;
            if (!use[i].active)
                continue;
            const uint16_t end = static_cast<uint16_t>(use[i].first) + use[i].count;
            if (use[i].count == 0 || end > boardChannels)
            {
                fault[i] = ChannelFault::OutOfRange;
                continue;
            }
            if (use[i].count == 2 && use[i].first % 2 != 0)
            {
                fault[i] = ChannelFault::PairStart;
                continue;
            }
            for (uint8_t c = use[i].first; c < end; c++)
            {
                if (owner[c] < 0)
                {
                    owner[c] = static_cast<int8_t>(i);
                    continue;
                }
                fault[i] = ChannelFault::Collision;
                if (fault[owner[c]] == ChannelFault::None)
                    fault[owner[c]] = ChannelFault::Collision;
            }
        }

        uint8_t faults = 0;
        for (uint8_t i = 0; i < n; i++)
            if (fault[i] != ChannelFault::None)
                faults++;
        return faults;
    }
} // namespace Kwl
