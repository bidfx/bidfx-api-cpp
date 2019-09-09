/**
 * Copyright 2018 2019 BidFX Systems Ltd. All rights reserved.
 */

#include "src/tools/buffer_util.h"
#include "src/tools/zlib_buffer_inflator.h"
#include "src/tools/hex_encoder.h"
#include "src/tools/byte_buffer.h"
#include "src/tools/zlib_buffer_compressor.h"
#include "gtest/gtest.h"

namespace bidfx_public_api::tools
{
class ZLIBBufferCompressorTest : public ::testing::Test
{
public:
    std::string SMALL_TEXT = "simple test of compression";
    std::string FRACTION_OF_BIGGER_TEXT = "DUCA:"
                                                       "Bella figlia dell'amore,"
                                                       "schiavo son de'vezzi tuoi;"
                                                       "con un detto, un detto sol"
                                                       "tu puoi le mie pene,"
                                                       "le mie pene consolar."
                                                       ""
                                                       "Vieni e senti del mio core"
                                                       "il frequente palpitar,"
                                                       "con un detto, un detto sol"
                                                       "tu puoi le mie pene,"
                                                       "le mie pene consolar.";
    std::string BIGGER_TEXT = "DUCA:"
                                           "Bella figlia dell'amore,"
                                           "schiavo son de'vezzi tuoi;"
                                           "con un detto, un detto sol"
                                           "tu puoi le mie pene,"
                                           "le mie pene consolar."
                                           ""
                                           "Vieni e senti del mio core"
                                           "il frequente palpitar,"
                                           "con un detto, un detto sol"
                                           "tu puoi le mie pene,"
                                           "le mie pene consolar."
                                           ""
                                           "MADDALENA:"
                                           "Ah! ah! rido ben di core, chè tai baie costan poco;"
                                           "GILDA:"
                                           "Ah! così parlar d'amore"
                                           "MADDALENA:"
                                           "Quanto valga il vostro gioco, mel credete, sò apprezzar."
                                           "GILDA:"
                                           "A me pur l'infame ho udito!"
                                           ""
                                           "RIGOLETTO: Taci, il piangere non vale;"
                                           "GILDA:     Infelice cor tradito,"
                                           "MADDALENA: Son avvezza, bel signore,"
                                           "DUCA:      Con un  detto"
                                           ""
                                           "RIGOLETTO: Taci,  taci il  piangere non  vale, no, non, val,"
                                           "GILDA:     Per angoscia non scoppiar,"
                                           "MADDALENA: Ad un simile scherzare,"
                                           "DUCA:      Sol tu  puoi"
                                           ""
                                           "RIGOLETTO: no, no, non, val."
                                           "GILDA:     no, no non, scoppiar."
                                           "MADDALENA: mio bel signor!"
                                           "DUCA:      le mie pene consolar. Bella figlia dell'amore,"
                                           ""
                                           "RIGOLETTO: Ch'ei mentiva,"
                                           "GILDA:     In felice"
                                           "MADDALENA: Ah! ah! rido ben di core, chè tai baie costan  poco,"
                                           "DUCA:      schiavo son de'vezzi tuoi;"
                                           ""
                                           "RIGOLETTO: Ch'ei mentiva,"
                                           "GILDA:     cor tradito,"
                                           "MADDALENA: Quanto valga il vostro gioco, mel credete, sò apprezzar."
                                           "DUCA:      Con un detto, un detto sol tu"
                                           ""
                                           "RIGOLETTO: sei sicura"
                                           "GILDA: Ah! No, non scoppiar."
                                           "MADDALENA: Sono avvezza, bel signore, ad un simile scherzare."
                                           "DUCA:      puoi le mie pene, le mie pene consolar."
                                           ""
                                           "RIGOLETTO: Taci, e mia sarà la cura"
                                           "GILDA:     In felice core, cor tradito,"
                                           "MADDALENA: Ah! Ah! Ah! Ah! Rido di cor,"
                                           "DUCA:      Ah! Con un detto"
                                           ""
                                           "RIGOLETTO: La vendetta d'affrettar,"
                                           "GILDA:     Per angoscia non scoppiare,"
                                           "MADDALENA: Ah! Ah! Rido di cor,"
                                           "DUCA:      sol tu puoi"
                                           ""
                                           "RIGOLETTO: Taci, e mia sarà la cura"
                                           "GILDA:     In felice cor tradito,"
                                           "MADDALENA: Ah, Ah, Rido di cor,"
                                           "DUCA:      Le mie pene,"
                                           ""
                                           "RIGOLETTO: La vendetta d'affrettar."
                                           "GILDA: Per angoscia non scoppiare,"
                                           "MADDALENA: Ah! Ah! Rido,"
                                           "DUCA:      consolar";
    std::vector<unsigned char> TROUBLING_BUFFER = HexEncoder::DecodeFromHex("700002003ff199999999999a013ff199999999999a");

    void TestCompression(std::string& text, uint32_t chunk_size);
    std::vector<unsigned char> TestCompression(std::vector<unsigned char>& input_buffer, uint32_t chunk_size);
    std::vector<unsigned char> InflateAndCheck(std::vector<unsigned char>& input_buffer, ByteBuffer& compressed);
    void DecompressAndCheck(std::string compressed1, std::string compressed2);
};

void ZLIBBufferCompressorTest::TestCompression(std::string& text, uint32_t chunk_size)
{
    std::vector<unsigned char> bytes = std::vector<unsigned char>(&text[0], &text[0] + text.size());
    std::vector<unsigned char> result = TestCompression(bytes, chunk_size);
    EXPECT_EQ(text, std::string(result.begin(), result.end()));
}

std::vector<unsigned char> ZLIBBufferCompressorTest::TestCompression(std::vector<unsigned char>& input_buffer, uint32_t chunk_size)
{
    ZLIBBufferCompressor compressor = ZLIBBufferCompressor(5);
    size_t count = 0;
    while (count < input_buffer.size())
    {
        size_t bytes_to_compress = std::min((size_t) chunk_size, input_buffer.size() - count);
        compressor.Compress(&input_buffer[0], count, bytes_to_compress);
        count += bytes_to_compress;
    }

    ByteBuffer compressed = compressor.GetCompressed();

    return InflateAndCheck(input_buffer, compressed);
}

std::vector<unsigned char> ZLIBBufferCompressorTest::InflateAndCheck(std::vector<unsigned char>& input_buffer,
                                                                     bidfx_public_api::tools::ByteBuffer& compressed)
{
    ZLIBBufferInflator decompressor;
    ByteBuffer decompressed = decompressor.Inflate(compressed);
    size_t num_bytes = decompressed.ReadableBytes();
    unsigned char* result_array = decompressed.ToArray();
    std::vector<unsigned char> result_vector = std::vector<unsigned char>(result_array, result_array + num_bytes);
    delete result_array;
    EXPECT_EQ(input_buffer, result_vector);
    return result_vector;
}

void ZLIBBufferCompressorTest::DecompressAndCheck(std::string compressed1, std::string compressed2)
{
    ZLIBBufferInflator decompressor1;
    ByteBuffer buffer1 = BufferUtil::HexBuffer(compressed1);
    ByteBuffer decompressed1 = decompressor1.Inflate(buffer1);
    std::unique_ptr<const char> decompressed_bytes1 = std::unique_ptr<const char>((const char*) decompressed1.ToArray());
    EXPECT_EQ(FRACTION_OF_BIGGER_TEXT, std::string(decompressed_bytes1.get(), decompressed_bytes1.get() +
            decompressed1.ReadableBytes()));

    ByteBuffer buffer2 = BufferUtil::HexBuffer(compressed2);
    ByteBuffer decompressed2 = decompressor1.Inflate(buffer2);
    std::unique_ptr<const char> decompressed_bytes2 = std::unique_ptr<const char>((const char*) decompressed2.ToArray());
    EXPECT_EQ(BIGGER_TEXT, std::string(decompressed_bytes2.get(), decompressed_bytes2.get() +
            decompressed2.ReadableBytes()));

}

TEST_F(ZLIBBufferCompressorTest, test_empty_buffer)
{
    std::vector<unsigned char> bytes;
    TestCompression(bytes, 65536);
}

TEST_F(ZLIBBufferCompressorTest, test_compression)
{
    TestCompression(SMALL_TEXT, 65536);
    TestCompression(BIGGER_TEXT, 65536);
}

TEST_F(ZLIBBufferCompressorTest, test_compression_in_chunks)
{
    TestCompression(SMALL_TEXT, 8);
    TestCompression(BIGGER_TEXT, 128);
}

TEST_F(ZLIBBufferCompressorTest, test_troubling_buffer)
{
    TestCompression(TROUBLING_BUFFER, 65536);
    TestCompression(TROUBLING_BUFFER, 8);
    TestCompression(TROUBLING_BUFFER, 65536);
    TestCompression(TROUBLING_BUFFER, 65536);
    TestCompression(TROUBLING_BUFFER, 65536);
}

TEST_F(ZLIBBufferCompressorTest, test_different_size_buffers_buffer)
{
    for (size_t i = 0; i < 256; i++)
    {
        std::vector<unsigned char> buffer;
        for (unsigned char j = 0; j < i; j++)
        {
            buffer.emplace_back(j);
        }
        TestCompression(buffer, 65536);
        TestCompression(buffer, 8);
        TestCompression(buffer, 65536);
        TestCompression(buffer, 65536);
        TestCompression(buffer, 65536);
    }
}

TEST_F(ZLIBBufferCompressorTest, test_with_precreated_compressed_arrays)
{
    // these were created by compressing data 1.7.0.51 or java 1.8.0_66 with the new way of flushing
    std::string small_input = "73696d706c652074657374206f6620636f6d7072657373696f6e";
    std::vector<unsigned char> bytes = HexEncoder::DecodeFromHex(small_input);
    ByteBuffer result1 = BufferUtil::HexBuffer("2acecc2dc8495528492d2e51c84f5348cecf2d284a2d2ececccf03000000ffff");
    InflateAndCheck(bytes, result1);

    std::string bigger_input = "445543413a42656c6c61206669676c69612064656c6c27616d6f72652c7363686961766f20736f6e2064652776657a7a692074756f693b636f6e20756e20646574746f2c20756e20646574746f20736f6c74752070756f69206c65206d69652070656e652c6c65206d69652070656e6520636f6e736f6c61722e5669656e6920652073656e74692064656c206d696f20636f7265696c206672657175656e74652070616c70697461722c636f6e20756e20646574746f2c20756e20646574746f20736f6c74752070756f69206c65206d69652070656e652c6c65206d69652070656e6520636f6e736f6c61722e4d414444414c454e413a41682120616821207269646f2062656e20646920636f72652c206368c3a820746169206261696520636f7374616e20706f636f3b47494c44413a41682120636f73c3ac207061726c6172206427616d6f72654d414444414c454e413a5175616e746f2076616c676120696c20766f7374726f2067696f636f2c206d656c20637265646574652c2073c3b22061707072657a7a61722e47494c44413a41206d6520707572206c27696e66616d6520686f20756469746f215249474f4c4554544f3a20546163692c20696c207069616e67657265206e6f6e2076616c653b47494c44413a2020202020496e66656c69636520636f72207472616469746f2c4d414444414c454e413a20536f6e20617676657a7a612c2062656c207369676e6f72652c445543413a202020202020436f6e20756e2020646574746f5249474f4c4554544f3a20546163692c20207461636920696c20207069616e67657265206e6f6e202076616c652c206e6f2c206e6f6e2c2076616c2c47494c44413a202020202050657220616e676f73636961206e6f6e2073636f70706961722c4d414444414c454e413a20416420756e2073696d696c652073636865727a6172652c445543413a202020202020536f6c207475202070756f695249474f4c4554544f3a206e6f2c206e6f2c206e6f6e2c2076616c2e47494c44413a20202020206e6f2c206e6f206e6f6e2c2073636f70706961722e4d414444414c454e413a206d696f2062656c207369676e6f7221445543413a2020202020206c65206d69652070656e6520636f6e736f6c61722e2042656c6c61206669676c69612064656c6c27616d6f72652c5249474f4c4554544f3a204368276569206d656e746976612c47494c44413a2020202020496e2066656c6963654d414444414c454e413a2041682120616821207269646f2062656e20646920636f72652c206368c3a820746169206261696520636f7374616e2020706f636f2c445543413a2020202020207363686961766f20736f6e2064652776657a7a692074756f693b5249474f4c4554544f3a204368276569206d656e746976612c47494c44413a2020202020636f72207472616469746f2c4d414444414c454e413a205175616e746f2076616c676120696c20766f7374726f2067696f636f2c206d656c20637265646574652c2073c3b22061707072657a7a61722e445543413a202020202020436f6e20756e20646574746f2c20756e20646574746f20736f6c2074755249474f4c4554544f3a207365692073696375726147494c44413a20416821204e6f2c206e6f6e2073636f70706961722e4d414444414c454e413a20536f6e6f20617676657a7a612c2062656c207369676e6f72652c20616420756e2073696d696c652073636865727a6172652e445543413a20202020202070756f69206c65206d69652070656e652c206c65206d69652070656e6520636f6e736f6c61722e5249474f4c4554544f3a20546163692c2065206d696120736172c3a0206c61206375726147494c44413a2020202020496e2066656c69636520636f72652c20636f72207472616469746f2c4d414444414c454e413a20416821204168212041682120416821205269646f20646920636f722c445543413a20202020202041682120436f6e20756e20646574746f5249474f4c4554544f3a204c612076656e64657474612064276166667265747461722c47494c44413a202020202050657220616e676f73636961206e6f6e2073636f7070696172652c4d414444414c454e413a2041682120416821205269646f20646920636f722c445543413a202020202020736f6c2074752070756f695249474f4c4554544f3a20546163692c2065206d696120736172c3a0206c61206375726147494c44413a2020202020496e2066656c69636520636f72207472616469746f2c4d414444414c454e413a2041682c2041682c205269646f20646920636f722c445543413a2020202020204c65206d69652070656e652c5249474f4c4554544f3a204c612076656e64657474612064276166667265747461722e47494c44413a2050657220616e676f73636961206e6f6e2073636f7070696172652c4d414444414c454e413a2041682120416821205269646f2c445543413a202020202020636f6e736f6c6172";
    bytes = HexEncoder::DecodeFromHex(bigger_input);
    ByteBuffer result2 = BufferUtil::HexBuffer(
            "ac54c16e1a3110fd95e1c4c5ca079013852842a2499bd0de27cb0023197b6befee215fd3630f3df517f8b13eef4231655142d5952cadedf1cc9bf79e3dfd32198f3e88b54c2b5d5b655a6232e4ad0f6262b1516e3c45efb03c6ce4f555a9aabdde1658a9d3625579f3e70f81b6aaa9440459a1ad0a95e2c464ff849388e270f355c52909457195a6aa88f1d80ea2965641bed5d8c021b6a5561ccc7f28f9713c9d8ee7770fe3d1783320c608baf4f42248a66d6943c566f7832a567a614d4763c58e4a5ff8dbfbd97cda9dc4eaee27900524a56547d631f7e79a1d80356cd74ce8a5418ee069ad4862688b468b20008f6271f78bb82c038805bc7d0184a09f4076a86ec5986c3cd54badfce06976ff38bf5b2c1e47b4e0424dca5e2abbb50421077e5053f638297d33b712ab45ea23501538653147a4f48c33dc245dd980064b51d72ed1304db66853d0a4e3bda3fb0c01a82a34e13805d22231f84dc399343519ae4f1208d13e16305c8a8f852f9120e4e0c6cb5437ea562126ac2801249d407bf61676a456fd0c5957f558f926abdcad775b87a2992f5a0f1e891864c57a1d4597ae4e0667b2198a4254d8bc6173a20e75f2e44d5f67ccd699392597afec7b105db0c93f3bfadc463dd717f0326c11c8a21675e03daec4c843a766af62f0b0ef373171af83725467ef46bfcc67b64f414c91c3ee3b41ff0cee89b007e9fa694d9de5e32969dee99d4b9ab672f6323073a6465c5ae5f40eadf06c56e9ad7ccf55933e281720742afd7dcfaea6a29f04d38e0b95e799366f377eb8e9d7379dd73cc8fe1b0000ffff");
    InflateAndCheck(bytes, result2);

    // these were created by compressing data in java 1.6.0.24 or 1.7.0.51 with the old way of flushing
    bytes = HexEncoder::DecodeFromHex(small_input);
    ByteBuffer result3 = BufferUtil::HexBuffer("2acecc2dc8495528492d2e51c84f5348cecf2d284a2d2ececccf0308000000ffff02");
    InflateAndCheck(bytes, result3);

    bytes = HexEncoder::DecodeFromHex(bigger_input);
    ByteBuffer result4 = BufferUtil::HexBuffer("ac54c16e1a3110fd95e1c4c5ca079013852842a2499bd0de27cb0023197b6befee215fd3630f3df517f8b13eef4231655142d5952cadedf1cc9bf79e3dfd32198f3e88b54c2b5d5b655a6232e4ad0f6262b1516e3c45efb03c6ce4f555a9aabdde1658a9d3625579f3e70f81b6aaa9440459a1ad0a95e2c464ff849388e270f355c52909457195a6aa88f1d80ea2965641bed5d8c021b6a5561ccc7f28f9713c9d8ee7770fe3d1783320c608baf4f42248a66d6943c566f7832a567a614d4763c58e4a5ff8dbfbd97cda9dc4eaee27900524a56547d631f7e79a1d80356cd74ce8a5418ee069ad4862688b468b20008f6271f78bb82c038805bc7d0184a09f4076a86ec5986c3cd54badfce06976ff38bf5b2c1e47b4e0424dca5e2abbb50421077e5053f638297d33b712ab45ea23501538653147a4f48c33dc245dd980064b51d72ed1304db66853d0a4e3bda3fb0c01a82a34e13805d22231f84dc399343519ae4f1208d13e16305c8a8f852f9120e4e0c6cb5437ea562126ac2801249d407bf61676a456fd0c5957f558f926abdcad775b87a2992f5a0f1e891864c57a1d4597ae4e0667b2198a4254d8bc6173a20e75f2e44d5f67ccd699392597afec7b105db0c93f3bfadc463dd717f0326c11c8a21675e03daec4c843a766af62f0b0ef373171af83725467ef46bfcc67b64f414c91c3ee3b41ff0cee89b007e9fa694d9de5e32969dee99d4b9ab672f6323073a6465c5ae5f40eadf06c56e9ad7ccf55933e281720742afd7dcfaea6a29f04d38e0b95e799366f377eb8e9d7379dd73cc8fe5b00000000ffff02");
    InflateAndCheck(bytes, result4);
}

TEST_F(ZLIBBufferCompressorTest, test_compression_of_big_buffer_in_chunks)
{
    std::vector<unsigned char> input_buffer;
    input_buffer.reserve(BIGGER_TEXT.size() * 1024);
    for (int i = 0; i < 1024; i++)
    {
        for (int j = 0; j < BIGGER_TEXT.size(); j++)
        {
            input_buffer.emplace_back(BIGGER_TEXT[j]);
        }
    }

    TestCompression(input_buffer, 65536);
}

TEST_F(ZLIBBufferCompressorTest, decompression_output_buffer_and_deflating_buffer_can_be_reused)
{
    ZLIBBufferCompressor compressor1 = ZLIBBufferCompressor(5);
    compressor1.Compress((unsigned char*) &SMALL_TEXT[0], 0, SMALL_TEXT.size());
    ByteBuffer compressed1 = compressor1.GetCompressed();

    ZLIBBufferInflator decompressor1;
    ByteBuffer decompressed1 = decompressor1.Inflate(compressed1);
    std::unique_ptr<const char> decompressed_bytes1 = std::unique_ptr<const char>((const char*) decompressed1.ToArray());
    ASSERT_EQ(SMALL_TEXT, std::string(decompressed_bytes1.get(), decompressed1.ReadableBytes()));

    ZLIBBufferCompressor compressor2 = ZLIBBufferCompressor(5);
    compressor2.Compress((unsigned char*) &SMALL_TEXT[0], 0, SMALL_TEXT.size());
    ByteBuffer compressed2 = compressor2.GetCompressed();

    ZLIBBufferInflator decompressor2;
    ByteBuffer decompressed2 = decompressor2.Inflate(compressed2);
    std::unique_ptr<const char> decompressed_bytes2 = std::unique_ptr<const char>((const char*) decompressed2.ToArray());
    EXPECT_EQ(SMALL_TEXT, std::string(decompressed_bytes2.get(), decompressed2.ReadableBytes()));
}

TEST_F(ZLIBBufferCompressorTest, compressor_and_decompressor_can_be_reused)
{
    ZLIBBufferCompressor compressor1 = ZLIBBufferCompressor(5);
    compressor1.Compress((unsigned char*) &FRACTION_OF_BIGGER_TEXT[0], 0, FRACTION_OF_BIGGER_TEXT.size());
    ByteBuffer compressed1 = compressor1.GetCompressed();

    ZLIBBufferInflator decompressor1;
    ByteBuffer decompressed1 = decompressor1.Inflate(compressed1);
    std::unique_ptr<const char> decompressed_bytes1 = std::unique_ptr<const char>((const char*) decompressed1.ToArray());
    ASSERT_EQ(FRACTION_OF_BIGGER_TEXT, std::string(decompressed_bytes1.get(), decompressed1.ReadableBytes()));

    compressor1.Compress((unsigned char*) &BIGGER_TEXT[0], 0, BIGGER_TEXT.size());
    ByteBuffer compressed2 = compressor1.GetCompressed();

    ByteBuffer decompressed2 = decompressor1.Inflate(compressed2);
    std::unique_ptr<const char> decompressed_bytes2 = std::unique_ptr<const char>((const char*) decompressed2.ToArray());
    EXPECT_EQ(BIGGER_TEXT, std::string(decompressed_bytes2.get(), decompressed2.ReadableBytes()));
}

TEST_F(ZLIBBufferCompressorTest, decompressor_can_be_reused_with_precalculated_compressed_buffer)
{// 1.6.0.24 or 1.7.0.51 with the old way of flushing
    DecompressAndCheck(
            "ac8d3b0ec2400c44afe22ecd8a0340c5e70ad05b6102969cf5b2eb4d91d3c7691007a09bcf1bcded7e3d1f2f50659ae4a5c2f40c33f06c15a98d6fe1c5a8598e7858b0ae42de4d4e63247d0fdd2d7d5580ea9d4a10a4a059400519e947532c83e27a7808b210a821bbecafc158d415a234557c7a1431622de25cd31f2e3701000000ffff02",
            "905305b96dc340f02beb532e441f909e8438080ca8499bf8031b9ab60928a440ca7a4f8e39f4d42ff8639d156d785d4b68dc6b0d10b0247267766696f3ffa9d96fd57c5ed5f78fd56db59d116325bf8af4ea50cc0fd086ec76ff411d7b7a652f4773c781da68e3d787453d2f27f176ff13cc128ad2aa8875aafd63c701c47a6e364ce8a5478d1469e351c4d01b1ab5c9813cc0f2fe1771db26080b7a07006c413f899a1b1fd68c876da4ddca7771f6bc7878aaef97cba75b5ab2f546aab79ec3c6254701fa00d31d7892fc1661ed1a6fa58f445d62a9624e4ce90567b8175fd9408686b2df04916188c55082ee8aee45ee0b0690ca7ae1714e646062f0575630f26814afef2e1176c76c1138d99f6d6c51206972d54a70b37ff33013517409229d517b890de24883fb8a59413d217f51c8e57df9740455b91832781262a6c046134553a3a3e8dc6d6f9c87a98879cfe6cc1d2af6e8a6af0be6904c2dc9f4c87e86d1444cfe39d197311a195fd053dc3298656f77890fbc4491c7e2e6a863c8701c0f31f1688234ab8b7b63dce68bd8cb26a6cc69ff4ef05fd13d33f668ddb8acd2995ecfe279f15b5b2a9fb47a8a4ccdd4bb206f59eea135aecd4eeecacf8c9a1ba33241a1b8f4e79c5d2dc5b808665813c8b5f2e6ef8d1f27fdfaa635e6d1f6df02000000ffff02");

    // 1.7.0.51 or 1.8.0_66 with the new of flushing
    DecompressAndCheck(
            "ac8d3b0ec2400c44afe22ecd8a0340c5e70ad05b6102969cf5b2eb4d91d3c7691007a09bcf1bcded7e3d1f2f50659ae4a5c2f40c33f06c15a98d6fe1c5a8598e7858b0ae42de4d4e63247d0fdd2d7d5580ea9d4a10a4a059400519e947532c83e27a7808b210a821bbecafc158d415a234557c7a1431622de25cd31f2e37000000ffff",
            "e454416edb3010fccafa940bd107a427210e02036ad226fec086a66d020a2990b2de93630e3df50bfe5867451b5ed7121af75a03042c89dc999d99e5fc7f6af65b359f57f5fd63755b6d67c458c9af22bd3a14f303b421bbdd7f50c79e5ed9cbd1dc71a036daf8f56151cfcb49bcddff04b384a2b42a629d6affd87100b19e9b0d137ae9512345da781431f486466d72200fb0bcff45dcb609c282de01005bd04fa2e6c68735e3611b69b7f25d9c3d2f1e9eeafbe5f2e996966cbd91eaade7b071c951803ec074079e24bf4558bbc65be923519758aa9813537ac119eec5573690a1a1ec37416418623194a0bba27b91fb8201a4b25e789c13199818fc95158c3c1ac5ebbb4b84dd315b044ef6671b5b14489a5cb512dcecdf3ccc44145d824867d45e628338d2e0be6256504fc85f1472795f3e1d41552e860c9e849829b0d144d1d4e8283a77db1be7612a62deb33973878a3dbae9eb823924534b323db29f613411937f4ef4658c46c617f414b70c66d9db5de2032f51e4b1b839ea18321cc7434c3c9a20cdeae2de18b7f922f6b2892973dabf13fc5774cf8c3d5a372eab74a6d7b3785efcd696ca27ad9e225333f52ec85b967b688d6bb393bbf233a3e6c6a84c50282efd3967574b312e8219d60472adbcf97be3c749bfbe698d79b4fd37000000ffff");
}

TEST_F(ZLIBBufferCompressorTest, reusing_the_same_compression_reduce_the_size_of_compression_when_compressing_the_same_input)
{
    ZLIBBufferCompressor compressor1 = ZLIBBufferCompressor(5);
    compressor1.Compress((unsigned char*) &BIGGER_TEXT[0], 0, BIGGER_TEXT.size());
    ByteBuffer compressed1 = compressor1.GetCompressed();
    size_t len1 = compressed1.ReadableBytes();

    ZLIBBufferInflator decompressor1;
    ByteBuffer decompressed1 = decompressor1.Inflate(compressed1);
    std::unique_ptr<const char> decompressed_bytes1 = std::unique_ptr<const char>((const char*) decompressed1.ToArray());
    ASSERT_EQ(BIGGER_TEXT, std::string(decompressed_bytes1.get(), decompressed1.ReadableBytes()));

    compressor1.Compress((unsigned char*) &BIGGER_TEXT[0], 0, BIGGER_TEXT.size());
    ByteBuffer compressed2 = compressor1.GetCompressed();
    size_t len2 = compressed2.ReadableBytes();
    EXPECT_LT(len2 * 10, len1);

    ByteBuffer decompressed2 = decompressor1.Inflate(compressed2);
    std::unique_ptr<const char> decompressed_bytes2 = std::unique_ptr<const char>((const char*) decompressed2.ToArray());
    EXPECT_EQ(BIGGER_TEXT, std::string(decompressed_bytes2.get(), decompressed2.ReadableBytes()));
}

TEST_F(ZLIBBufferCompressorTest, it_fails_if_decompressor_instance_is_not_the_same)
{
    ZLIBBufferCompressor compressor1 = ZLIBBufferCompressor(5);
    compressor1.Compress((unsigned char*) &FRACTION_OF_BIGGER_TEXT[0], 0, FRACTION_OF_BIGGER_TEXT.size());
    ByteBuffer compressed1 = compressor1.GetCompressed();

    ZLIBBufferInflator decompressor1;
    ByteBuffer decompressed1 = decompressor1.Inflate(compressed1);
    std::unique_ptr<const char> decompressed_bytes1 = std::unique_ptr<const char>((const char*) decompressed1.ToArray());
    ASSERT_EQ(FRACTION_OF_BIGGER_TEXT, std::string(decompressed_bytes1.get(), decompressed1.ReadableBytes()));

    compressor1.Compress((unsigned char*) &BIGGER_TEXT[0], 0, BIGGER_TEXT.size());
    ByteBuffer compressed2 = compressor1.GetCompressed();

    ZLIBBufferInflator decompressor2;
    EXPECT_THROW(decompressor2.Inflate(compressed2), std::exception);
}
}