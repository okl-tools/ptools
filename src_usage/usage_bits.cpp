#include "PTools.h"
#include "PBits.h"
#include "PStringFixedSize.h"


using namespace ptools;


enum FLAGS
{
    FLAGS_BOLD = 1 << 1,
    FLAGS_ITALIC = 1 << 2,
    FLAGS_UNDERLINE = 1 << 3,
};

static void show_flags(const Bits32 & bits)
{
    PStringFixedSize s;

    if (bits.is_flag(FLAGS_ITALIC))
    {
        s.sprintF("ITALIC | ");
    }

    if (bits.is_flag(FLAGS_BOLD))
    {
        s.sprintF("BOLD | ");
    }
    if (bits.is_flag(FLAGS_UNDERLINE))
    {
        s.sprintF("UNDERLINE");
    }

    pp("font flags:$", s);

}

static void test_bits_font ()
{
    Bits32 bits;
    bits.set_flag(FLAGS_ITALIC);

    show_flags(bits);

    bits.set_flag(FLAGS_BOLD);
    show_flags(bits);

    bits.set_flag(FLAGS_UNDERLINE);
    show_flags(bits);

    bits.set_flag(FLAGS_UNDERLINE, false);
    show_flags(bits);


}

int main ()
{
//    logWriter.flagWriteTrailer = false;
    logWriter.flagWriteTime = true;
    pp("ptools version $.$.$", version::major(), version::minor(), version::patch());
    sleep_milliseconds(300);
    test_bits_font();

    return 0;
}