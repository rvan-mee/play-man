#pragma once

/**
 * @brief These are the new licensing codes inside the header of a ROM.
 * They are encoded as 2-byte values, representing ascii characters.
 * 
 * @note If we take Nintendo's R&D licensing code as an example, it is encoded as the ascii values 0 and 1.
 * If we translate these ascii values to decimal values we end up with 48 and 49.
 * Then translating those to hex we get 0x30 and 0x31.
 * 
 * The final encoding will be 0x3031, we have separated the two bytes with a \' to make reading the values easier.
 */
enum class NewLicensingCodes {
    None                                    = 0x30'30, // "00"
    Nintendo_Research_and_Development_1     = 0x30'31, // "01"
    Capcom                                  = 0x30'38, // "08"
    EA_Electronic_Arts                      = 0x31'33, // "13"
    Hudson_Soft                             = 0x31'38, // "18"
    B_AI                                    = 0x31'39, // "19"
    KSS                                     = 0x32'30, // "20"
    Planning_Office_WADA                    = 0x32'32, // "22"
    PCM_Complete                            = 0x32'34, // "24"
    San_X                                   = 0x32'35, // "25"
    Kemco                                   = 0x32'38, // "28"
    SETA_Corporation                        = 0x32'39, // "29"
    Viacom                                  = 0x33'30, // "30"
    Nintendo                                = 0x33'31, // "31"
    Bandai                                  = 0x33'32, // "32"
    Ocean_Software_Acclaim_Entertainment    = 0x33'33, // "33"
    Konami                                  = 0x33'34, // "34"
    HectorSoft                              = 0x33'35, // "35"
    Taito                                   = 0x33'37, // "37"
    Hudson_Soft_2                           = 0x33'38, // "38"
    Banpresto                               = 0x33'39, // "39"
    Ubi_Soft1                               = 0x34'31, // "41"
    Atlus                                   = 0x34'32, // "42"
    Malibu_Interactive                      = 0x34'34, // "44"
    Angel                                   = 0x34'36, // "46"
    Bullet_Proof_Software                   = 0x34'37, // "47"
    Irem                                    = 0x34'39, // "49"
    Absolute                                = 0x35'30, // "50"
    Acclaim_Entertainment                   = 0x35'31, // "51"
    Activision                              = 0x35'32, // "52"
    Sammy_USA_Corporation                   = 0x35'33, // "53"
    Hi_Tech_Expressions                     = 0x35'35, // "55"
    LJN                                     = 0x35'36, // "56"
    Matchbox                                = 0x35'37, // "57"
    Mattel                                  = 0x35'38, // "58"
    Milton_Bradley_Company                  = 0x35'39, // "59"
    Titus_Interactive                       = 0x36'30, // "60"
    Virgin_Games_Ltd3                       = 0x36'31, // "61"
    Lucasfilm_Games4                        = 0x36'34, // "64"
    Ocean_Software_2                        = 0x36'37, // "67"
    EA_Electronic_Arts_2                    = 0x36'39, // "69"
    Infogrames5                             = 0x37'30, // "70"
    Interplay_Entertainment                 = 0x37'31, // "71"
    Broderbund                              = 0x37'32, // "72"
    Sculptured_Software                     = 0x37'33, // "73"
    The_Sales_Curve_Limited                 = 0x37'35, // "75"
    THQ                                     = 0x37'38, // "78"
    Accolade                                = 0x37'39, // "79"
    Misawa_Entertainment                    = 0x38'30, // "80"
    lozc                                    = 0x38'33, // "83"
    Tokuma_Shoten                           = 0x38'36, // "86"
    Tsukuda_Original                        = 0x38'37, // "87"
    Chunsoft_Co8                            = 0x39'31, // "91"
    Video_System                            = 0x39'32, // "92"
    Ocean_Software_Acclaim_Entertainment_2  = 0x39'33, // "93"
    Varie                                   = 0x39'35, // "95"
    Yonezawa_s_pal                          = 0x39'36, // "96"
    Kaneko                                  = 0x39'37, // "97"
    Pack_In_Video                           = 0x39'39, // "99"
    Bottom_Up                               = 0x39'48, // "9H"
    Konami_Yu_Gi_Oh                         = 0x41'34, // "A4"
    MTO                                     = 0x42'4C, // "BL"
    Kodansha                                = 0x44'4B  // "DK"
};

/**
 * @brief These are the values used to check what cartridge type is inserted.
 */
enum class CartridgeType {
    ROM_ONLY                        = 0x00, // No MBC, only ROM, no memory mapping, no battery
    MBC1                            = 0x01, // Basic memory bank controller, 2 ROM banks, up to 2 RAM banks
    MBC1_RAM                        = 0x02, // MBC1 with RAM support (but no battery)
    MBC1_RAM_BATTERY                = 0x03, // MBC1 with RAM and battery backup
    MBC2                            = 0x05, // Simple MBC used in some early GameBoy games, supports 512 bytes of RAM
    MBC2_BATTERY                    = 0x06, // MBC2 with battery backup for saved data
    ROM_RAM                         = 0x08, // ROM and RAM memory without a battery
    ROM_RAM_BATTERY                 = 0x09, // ROM and RAM with battery backup
    MMM01                           = 0x0B, // Memory mapper for some third-party games
    MMM01_RAM                       = 0x0C, // MMM01 with additional RAM
    MMM01_RAM_BATTERY               = 0x0D, // MMM01 with RAM and battery support
    MBC3_TIMER_BATTERY              = 0x0F, // MBC3 with a real-time clock (RTC) and battery backup
    MBC3_TIMER_RAM_BATTERY          = 0x10, // MBC3 with RTC, RAM, and battery backup
    MBC3                            = 0x11, // MBC3 with up to 8 ROM banks and 8 RAM banks, typically used in RPGs
    MBC3_RAM                        = 0x12, // MBC3 with RAM support
    MBC3_RAM_BATTERY                = 0x13, // MBC3 with RAM and battery backup
    MBC5                            = 0x19, // MBC5, a more advanced memory bank controller with 32k RAM support, common in later games
    MBC5_RAM                        = 0x1A, // MBC5 with RAM support
    MBC5_RAM_BATTERY                = 0x1B, // MBC5 with RAM and battery backup
    MBC5_RUMBLE                     = 0x1C, // MBC5 with rumble pack support
    MBC5_RUMBLE_RAM                 = 0x1D, // MBC5 with rumble and RAM support
    MBC5_RUMBLE_RAM_BATTERY         = 0x1E, // MBC5 with rumble, RAM, and battery backup
    MBC6                            = 0x20, // A memory bank controller that supports more advanced features, less common
    MBC7_SENSOR_RUMBLE_RAM_BATTERY  = 0x22, // MBC7 with sensor and rumble, RAM, and battery support
    POCKET_CAMERA                   = 0xFC, // A special type for the GameBoy Pocket Camera accessory
    BANDAI_TAMA5                    = 0xFD, // Memory mapping used for Bandai’s TAMA5 system games
    HUC3                            = 0xFE, // A special memory controller used in some games, notably for the Hudson Soft games
    HUC1_RAM_BATTERY                = 0xFF  // HuC1 controller with RAM and battery backup (used in some RPGs)
};

/**
 * @brief The amount of ROM bytes present on the cartridge.
 * The total size can be found by doing "32 KiB × (1 << <value>)". 
 */
enum class RomSize {
                        // Size,    Number of ROM banks
    KiB32     = 0x00,	// 32 KiB	2 (no banking)
    Kib64     = 0x01,	// 64 KiB	4
    Kib128    = 0x02,	// 128 KiB	8
    Kib256    = 0x03,	// 256 KiB	16
    KiB512    = 0x04,	// 512 KiB	32
    MiB1      = 0x05,	// 1 MiB	64
    MiB2      = 0x06,	// 2 MiB	128
    MiB4      = 0x07,	// 4 MiB	256
    MiB8      = 0x08,	// 8 MiB	512
};

/**
 * @brief How much RAM is present on the cartridge, if any.
 * If the cartridge type does not include “RAM” in its name, this should be set to 0
 */
enum class RamSize {
                    // Size,    Number of RAM banks
    NoRam   = 0x00,	// 0        No Banks
    Unused  = 0x01,	// -        Unused
    KiB8    = 0x02,	// 8 KiB    1 bank
    KiB32   = 0x03,	// 32 KiB	4 banks of 8 KiB each
    KiB128  = 0x04,	// 128 KiB	16 banks of 8 KiB each
    KiB64   = 0x05,	// 64 KiB	8 banks of 8 KiB each
};

/**
 * @brief This byte specifies whether this version of the game is intended to be sold in Japan or elsewhere.
 */
enum class DestinationCode {
    Japan       = 0x00, // This cartridge was meant to be sold in Japan (and possibly overseas) 
    Overseas    = 0x01, // This cartridge was meant to be sold overseas
};

// TODO: doublecheck the names and values, remove some numbers at the end of names
enum class OldLicensingCodes {
    None                    = 0x00,
    Nintendo                = 0x01,
    Capcom                  = 0x08,
    HOT_B                   = 0x09,
    Jaleco                  = 0x0A,
    Coconuts_Japan          = 0x0B,
    Elite_Systems           = 0x0C,
    EA_Electronic_Arts      = 0x13,
    Hudson_Soft             = 0x18,
    ITC_Entertainment       = 0x19,
    Yanoman                 = 0x1A,
    Japan_Clary             = 0x1D,
    Virgin_Games_Ltd3       = 0x1F,
    PCM_Complete            = 0x24,
    San_X                   = 0x25,
    Kemco                   = 0x28,
    SETA_Corporation        = 0x29,
    Infogrames5             = 0x30,
    Nintendo_2              = 0x31,
    Bandai                  = 0x32,
    New_Licensee_Code      = 0x33,
    Konami                  = 0x34,
    HectorSoft              = 0x35,
    Capcom_2                = 0x38,
    Banpresto               = 0x39,
    Entertainment_Interactive = 0x3C,
    Gremlin                 = 0x3E,
    Ubi_Soft1               = 0x41,
    Atlus                   = 0x42,
    Malibu_Interactive      = 0x44,
    Angel                   = 0x46,
    Spectrum_HoloByte       = 0x47,
    Irem                    = 0x49,
    Virgin_Games_Ltd4       = 0x4A,
    Malibu_Interactive_2    = 0x4D,
    US_Gold                 = 0x4F,
    Absolute                = 0x50,
    Acclaim_Entertainment   = 0x51,
    Activision              = 0x52,
    Sammy_USA_Corporation   = 0x53,
    GameTek                 = 0x54,
    Park_Place13            = 0x55,
    LJN                     = 0x56,
    Matchbox                = 0x57,
    Milton_Bradley_Company  = 0x59,
    Mindscape               = 0x5A,
    Romstar                 = 0x5B,
    Naxat_Soft14            = 0x5C,
    Tradewest               = 0x5D,
    Titus_Interactive       = 0x60,
    Virgin_Games_Ltd5       = 0x61,
    Ocean_Software          = 0x67,
    EA_Electronic_Arts_2    = 0x69,
    Elite_Systems_2         = 0x6E,
    Electro_Brain           = 0x6F,
    Infogrames5_2           = 0x70,
    Interplay_Entertainment = 0x71,
    Broderbund              = 0x72,
    Sculptured_Software6    = 0x73,
    Sales_Curve_Limited7    = 0x75,
    THQ                     = 0x78,
    Accolade                = 0x79,
    Triffix_Entertainment   = 0x7A,
    MicroProse              = 0x7C,
    Kemco_2                 = 0x7F,
    Misawa_Entertainment    = 0x80,
    LOZC_G                  = 0x83,
    Tokuma_Shoten           = 0x86,
    Bullet_Proof_Software2  = 0x8B,
    Vic_Tokai_Corp16        = 0x8C,
    Ape_Inc17               = 0x8E,
    I_Max18                 = 0x8F,
    Chunsoft_Co8           = 0x91,
    Video_System            = 0x92,
    Tsubaraya_Productions   = 0x93,
    Varie                   = 0x95,
    Yonezawa_S_Pal          = 0x96,
    Kemco_3                 = 0x97,
    Arc                     = 0x99,
    Nihon_Bussan            = 0x9A,
    Tecmo                   = 0x9B,
    Imagineer               = 0x9C,
    Banpresto_2             = 0x9D,
    Nova                    = 0x9F,
    Hori_Electric           = 0xA1,
    Bandai_2                = 0xA2,
    Konami_2                = 0xA4,
    Kawada                  = 0xA6,
    Takara                  = 0xA7,
    Technos_Japan           = 0xA9,
    Broderbund_2            = 0xAA,
    Toei_Animation          = 0xAC,
    Toho                    = 0xAD,
    Namco                   = 0xAF,
    Acclaim_Entertainment_2 = 0xB0,
    ASCII_Corp_or_Nexsoft   = 0xB1,
    Bandai_3                = 0xB2,
    Square_Enix             = 0xB4,
    HAL_Laboratory          = 0xB6,
    SNK                     = 0xB7,
    Pony_Canyon             = 0xB9,
    Culture_Brain           = 0xBA,
    Sunsoft                 = 0xBB,
    Sony_Imagesoft          = 0xBD,
    Sammy_Corp              = 0xBF,
    Taito_2                 = 0xC0,
    Kemco_4                 = 0xC2,
    Square                  = 0xC3,
    Tokuma_Shoten_2         = 0xC4,
    Data_East               = 0xC5,
    Tonkin_House            = 0xC6,
    Koei                    = 0xC8,
    UFL                     = 0xC9,
    Ultra_Games             = 0xCA,
    VAP_Inc                 = 0xCB,
    Use_Corp                = 0xCC,
    Meldac                  = 0xCD,
    Pony_Canyon_2           = 0xCE,
    Angel_2                 = 0xCF,
    Taito_3                 = 0xD0,
    SOFEL                   = 0xD1,
    Quest                   = 0xD2,
    Sigma_Enterprises       = 0xD3,
    ASK_Kodansha_Co         = 0xD4,
    Naxat_Soft15            = 0xD6,
    Copya_System            = 0xD7,
    Banpresto_3             = 0xD9,
    Tomy                    = 0xDA,
    LJN_2                   = 0xDB,
    Nippon_Computer_Systems = 0xDD,
    Human_Ent               = 0xDE,
    Altron                  = 0xDF,
    Jaleco_2                = 0xE0,
    Towa_Chiki              = 0xE1,
    Yutaka                  = 0xE2,
    Varie_2                 = 0xE3,
    Epoch                   = 0xE5,
    Athena                  = 0xE7,
    Asmik_Ace_Entertainment = 0xE8,
    Natsume                 = 0xE9,
    King_Records            = 0xEA,
    Atlus_2                 = 0xEB,
    Epic_Sony_Records       = 0xEC,
    IGS                     = 0xEE,
    A_Wave                  = 0xF0,
    Extreme_Entertainment   = 0xF3,
    LJN_3                   = 0xFF,
}; 
