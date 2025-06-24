#include "PTools.h"
#include "PStringFixedSize.h"


using namespace ptools;

struct Baum
{
    uint32_t age = 0;
    char name[64];

    Baum (uint32_t age, const char * pName) : age(age)
    {
        if (pName)
        {
            string_copy(name, pName);

        }
    }


    PStringFixedSize<64> to_pw_string() const
    {
        PStringFixedSize<64> pw;
        pw.sprintF("name:$, age:$", name, age);
        return pw;
    };

//    void to_writer(PWriter & writer) const
//    {
//        writer.sprintF("EICHE");
//    }

};


static void test1 ()
{
    PWriterConsole pc;
    pc.write_cstring("wert");
//    pc.write(345U);
    pc.write(3);
    pc.write(2L);
    pc.write("Blubber");
    pc.write(2L);
    pc.write('t');
    pc.write(0.456f);
    pc.write("PI:");
    pc.write(3.145635);
    pc.write("X");
    pc.write_LF();

}

static void test2 ()
{
    PWriterConsole pc;
    pc.sprintF("Jodel $ war da und $ auch $", 3, "müll", 3.1456);
    pc.write_LF();
    const char * pColor = "lila";
    pc.sprintF("Die Farbe $ ist nicht mehr in Mode im Jahre $", pColor, 2025);
    pc.write_LF();

    Baum baum(45, "Eiche");

    PStringFixedSize<64> s = baum.to_pw_string();


//    pc.sprintF("Die $ , hat eine Höhe von $ Metern", baum, 28);
    pc.sprintF("Die $ , hat eine Höhe von $ Metern", s, 28);
    pc.write_LF();

}

static void test_string_del()
{
    PStringFixedSize<64> s;
//    s.sprintF("Der Himmel ist blau!\r\nUnd so schön");
//    s.sprintF("Der Himmel ist blau!\r\n");
    s.sprintF("Das");

    pp("<<<$>>>", s);
    pp("pos:$", s.get_pos());

    s.sprintF(" ABC\r\n");
//    s.sprintF("Das ABC");
//    s.sprintF("Das ABC");
    //s.sprintF("");
    //s.reset();

//    pp("'$'", s);
//    s.remove_chars(4, 10);
//    s.remove_first_found_char(0, 'H');
    s.remove_from_end_CRLF();

    s.sprintF("MURKS\n");
    s.remove_from_end_CRLF();
    s.sprintF("jodel\n");
    s.remove_from_end_CRLF();
    pp("<<<$>>>", s);
//    pp("$verdammt <<<$>>>, $", 1, s, 2);

}

static void test_construct()
{
    const char * pABC = "abcdefghijklmnopqrstuvwxyz";
    PStringFixedSize<26> s1(pABC); // not all 26 letters because we need a 0 at the end of the string
    pp("s1 <<<$>>>, size:$, capacity:$", s1, s1.size(), s1.get_capacity());

    PStringFixedSize<27> s2(pABC);
    pp("s2 <<<$>>>, size:$, capacity:$", s2, s2.size(), s2.get_capacity());

    PStringFixedSize<128> s3(pABC);
    pp("s3 <<<$>>>, size:$, capacity:$", s3, s3.size(), s3.get_capacity());

}

static void test_insert()
{
    const char * pABC = "abcdefghijklmnopqrstuvwxyz";
    PStringFixedSize<128> s3(pABC);
    pp("s3 <<<$>>>, size:$, capacity:$", s3, s3.size(), s3.get_capacity());

    const char * pPart = "-it is hot today-";
    s3.insert_at(3, pPart, string_len(pPart));

    pp("s3 <<<$>>>, size:$, capacity:$", s3, s3.size(), s3.get_capacity());

}

static void testPWriterString ()
{
    pp("main_PWriter::testPWriterString");
    PStringFixedSize<64> pw;
    pw.sprintF("name:$, age:$!", "Hugo", 99);

    pp(4, "pw:$", pw);

    PStringFixedSize<28> pw2;
//    pw2.sprintF("urk $ war da!", 4.78);
//    pw2 += pw.get_data();
    pw2 += pw;
    pw2 += "abc";
    pw2 += "abc";
    pw2 += "abc";

    pp(4, "pw2:$", pw2);

    pw2 = "zoff";

    pp(4, "pw2:$", pw2);

    pw += "Was für ein Schrott hier!";

//    pw2 = pw.get_data();
    pw2 = pw;
    pp(4, "pw2:$", pw2);

    pw = "99";

//    pp("val:$", pw.to_number<uint32_t>());
    pp("val:$", pw.to_int());

    pw = "hurk";


    if (pw == "hurk")
    {
        pp("aha, hurk");
    }

    pw2 = "hurk";

    if (pw.equals(pw2.get_data()))
    {
        pp("aha, pw == pw2");

    }



}



int main ()
{
//    test_string_del();
    test_construct();
//    test2();
//    testPWriterString();

    return 0;
}


