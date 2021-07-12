#include "calculator.h"

Calculator::Calculator()
{

}

void Calculator::charToByte(bytes out[], const char s[])
{
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 8; ++j)
            out[i][j] = ((s[i] >> j) & 1);
}

bitset<128> Calculator::mergeByte(bytes in[])
{
    bitset<128> res;
    res.reset();  // 置0
    bitset<128> temp;
    for (int i = 0; i < 16; ++i)
    {
        temp = in[i].to_ulong();
        temp <<= 8 * (15 - i);
        res |= temp;
    }
    return res;
}

void Calculator::divideToByte(bytes out[16], bitset<128>& data)
{
    bitset<128> temp;
    for (int i = 0; i < 16; ++i)
    {
        temp = (data << 8 * i) >> 120;
        out[i] = temp.to_ulong();
    }
}

void Calculator::KeyExpansion(bytes key[], word w[])
{
    word temp;
    int i = 0;
    while (i < N_key)   //前四个word就是输入的key
    {
        w[i] = ToWord(key[4 * i], key[4 * i + 1], key[4 * i + 2], key[4 * i + 3]);
        ++i;
    }
    i = N_key;
    while (i < 4 * (N_round + 1))
    {
        temp = w[i - 1]; //记录前一个word
        if (i % N_key == 0)
        { //temp先位置表换RotWord，再S盒变换，然后与轮常数异或，最后w[i-N_key] 异或
            w[i] = w[i - N_key] ^ SubWord(RotWord(temp)) ^ Rcon[i / N_key - 1];
        }
        else
        {
            w[i] = w[i - N_key] ^ temp;
        }
        i++;
    }
}
word Calculator::SubWord(const word &sw)
{
    word temp;
    for (int i = 0; i < 32; i += 8)
    {
        int row = sw[i + 7] * 8 + sw[i + 6] * 4 + sw[i + 5] * 2 + sw[i + 4];
        int col = sw[i + 3] * 8 + sw[i + 2] * 4 + sw[i + 1] * 2 + sw[i];
        bytes val = S_Box[row][col];
        for (int j = 0; j < 8; ++j)
            temp[i + j] = val[j];
    }
    return temp;
}

word Calculator::ToWord(bytes &k1, bytes &k2, bytes &k3, bytes &k4)
{
    word result(0x00000000);
    word temp;
    temp = k1.to_ulong();  // K1
    temp <<= 24;
    result |= temp;
    temp = k2.to_ulong();  // K2
    temp <<= 16;
    result |= temp;
    temp = k3.to_ulong();  // K3
    temp <<= 8;
    result |= temp;
    temp = k4.to_ulong();  // K4
    result |= temp;
    return result;
}

word Calculator::RotWord(const word &rw)
{
    word high = rw << 8;
    word low = rw >> 24;
    return high | low;
}

void Calculator::AddRoundKey(bytes mtx[], word k[])
{
    for (int i = 0; i < 4; ++i)
    {
        word k1 = k[i] >> 24;
        word k2 = (k[i] << 8) >> 24;
        word k3 = (k[i] << 16) >> 24;
        word k4 = (k[i] << 24) >> 24;

        mtx[i] = mtx[i] ^ bytes(k1.to_ulong());
        mtx[i + 4] = mtx[i + 4] ^ bytes(k2.to_ulong());
        mtx[i + 8] = mtx[i + 8] ^ bytes(k3.to_ulong());
        mtx[i + 12] = mtx[i + 12] ^ bytes(k4.to_ulong());
    }
}

bytes Calculator::GFMul(bytes a, bytes b)
{
    bytes p = 0;
    bytes hi_bit_set;
    for (int counter = 0; counter < 8; counter++)
    {
        if ((b & bytes(1)) != 0)
        {
            p ^= a;
        }
        hi_bit_set = (bytes)(a & bytes(0x80));
        a <<= 1;
        if (hi_bit_set != 0)
        {
            a ^= 0x1b; // x^8 + x^4 + x^3 + x + 1
        }
        b >>= 1;
    }
    return p;
}

void Calculator::SubBytes(bytes mtx[4 * 4])
{
    for (int i = 0; i < 16; ++i)
    {
        int row = mtx[i][7] * 8 + mtx[i][6] * 4 + mtx[i][5] * 2 + mtx[i][4];
        int col = mtx[i][3] * 8 + mtx[i][2] * 4 + mtx[i][1] * 2 + mtx[i][0];
        mtx[i] = S_Box[row][col];
    }
}

void Calculator::ShiftRows(bytes mtx[4 * 4])
{
    // 第二行循环左移一位
    bytes temp = mtx[4];
    for (int i = 0; i < 3; ++i)
        mtx[i + 4] = mtx[i + 5];
    mtx[7] = temp;
    // 第三行循环左移两位
    for (int i = 0; i < 2; ++i)
    {
        temp = mtx[i + 8];
        mtx[i + 8] = mtx[i + 10];
        mtx[i + 10] = temp;
    }
    // 第四行循环左移三位
    temp = mtx[15];
    for (int i = 3; i > 0; --i)
        mtx[i + 12] = mtx[i + 11];
    mtx[12] = temp;
}

void Calculator::MixColumns(bytes mtx[])
{
    bytes arr[4];
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
            arr[j] = mtx[i + j * 4];

        mtx[i] = GFMul(0x02, arr[0]) ^ GFMul(0x03, arr[1]) ^ arr[2] ^ arr[3];
        mtx[i + 4] = arr[0] ^ GFMul(0x02, arr[1]) ^ GFMul(0x03, arr[2]) ^ arr[3];
        mtx[i + 8] = arr[0] ^ arr[1] ^ GFMul(0x02, arr[2]) ^ GFMul(0x03, arr[3]);
        mtx[i + 12] = GFMul(0x03, arr[0]) ^ arr[1] ^ arr[2] ^ GFMul(0x02, arr[3]);
    }
}

void Calculator::encrypt(bytes in[], word w[])
{
    word key[4];
    for (int i = 0; i < 4; ++i)
    {
        key[i] = w[i];
    }
    AddRoundKey(in, key);

    for (int round = 1; round < N_round; ++round)
    {
        SubBytes(in);
        ShiftRows(in);
        MixColumns(in);
        for (int i = 0; i < 4; ++i)
        {
            key[i] = w[4 * round + i];
        }

        AddRoundKey(in, key);
    }

    SubBytes(in);
    ShiftRows(in);
    for (int i = 0; i < 4; ++i)
    {
        key[i] = w[4 * N_round + i];
    }
    AddRoundKey(in, key);
}

void Calculator::InvSubBytes(bytes mtx[])
{
    for (int i = 0; i < 16; ++i)
    {
        int row = mtx[i][7] * 8 + mtx[i][6] * 4 + mtx[i][5] * 2 + mtx[i][4];
        int col = mtx[i][3] * 8 + mtx[i][2] * 4 + mtx[i][1] * 2 + mtx[i][0];
        mtx[i] = Inv_S_Box[row][col];
    }
}

void Calculator::InvShiftRows(bytes mtx[])
{
    // 第二行循环右移一位
    bytes temp = mtx[7];
    for (int i = 3; i > 0; --i)
    {
        mtx[i + 4] = mtx[i + 3];
    }
    mtx[4] = temp;
    // 第三行循环右移两位
    for (int i = 0; i < 2; ++i)
    {
        temp = mtx[i + 8];
        mtx[i + 8] = mtx[i + 10];
        mtx[i + 10] = temp;
    }
    // 第四行循环右移三位
    temp = mtx[12];
    for (int i = 0; i < 3; ++i)
    {
        mtx[i + 12] = mtx[i + 13];
    }
    mtx[15] = temp;
}

void Calculator::InvMixColumns(bytes mtx[])
{
    bytes arr[4];
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
            arr[j] = mtx[i + j * 4];
        mtx[i] = GFMul(0x0e, arr[0]) ^ GFMul(0x0b, arr[1]) ^ GFMul(0x0d, arr[2]) ^ GFMul(0x09, arr[3]);
        mtx[i + 4] = GFMul(0x09, arr[0]) ^ GFMul(0x0e, arr[1]) ^ GFMul(0x0b, arr[2]) ^ GFMul(0x0d, arr[3]);
        mtx[i + 8] = GFMul(0x0d, arr[0]) ^ GFMul(0x09, arr[1]) ^ GFMul(0x0e, arr[2]) ^ GFMul(0x0b, arr[3]);
        mtx[i + 12] = GFMul(0x0b, arr[0]) ^ GFMul(0x0d, arr[1]) ^ GFMul(0x09, arr[2]) ^ GFMul(0x0e, arr[3]);
    }
}

void Calculator::decrypt(bytes in[], word w[])
{
    word key[4];
    for (int i = 0; i < 4; ++i)
    {
        key[i] = w[4 * N_round + i];
    }
    AddRoundKey(in, key);

    for (int round = N_round - 1; round > 0; --round)
    {
        InvShiftRows(in);
        InvSubBytes(in);
        for (int i = 0; i < 4; ++i)
            key[i] = w[4 * round + i];
        AddRoundKey(in, key);
        InvMixColumns(in);
    }

    InvShiftRows(in);
    InvSubBytes(in);
    for (int i = 0; i < 4; ++i)
    {
        key[i] = w[i];
    }
    AddRoundKey(in, key);
}

void Calculator::aesTest()
{
    string KeyStr = "abcdefghijklmnop";
    bytes key[16];
    charToByte(key, KeyStr.c_str());
    word w[4 * (N_round + 1)];
    KeyExpansion(key, w);

    bitset<128> data;
    bytes plain[16];

    string readFilePath = "/home/lbw/Desktop/png/screenshot.png";
    string resultFilePath = "/home/lbw/Desktop/png/result.png";
    string encryptFilePath = "/home/lbw/Desktop/png/cipher.txt";

    QFileInfo fileInfo(QString::fromStdString(readFilePath));

    string addNumber = "";
    bool isTxtFile = (fileInfo.suffix() == "txt");
    if (isTxtFile)
    {
        ifstream judgeSizeIn(readFilePath, ios::binary);
        stringstream buffer;
        buffer << judgeSizeIn.rdbuf();
        string s = buffer.str();
        int jLength = s.size() % 16;
        if (jLength != 0)
        {
            int number;
            if ((16 - jLength) > 9)
            {
                number = 2;
            }
            else
            {
                number = 1;
            }
            for (int i = 0; i < (16 -jLength - number); i++)
            {
                addNumber += "0";
            }
            string j_Str = to_string(16 - jLength);
            addNumber += j_Str;
            ofstream judgeSizeOut(readFilePath, ios::binary | ios::app);
            judgeSizeOut << addNumber;
            judgeSizeOut.close();
        }
        judgeSizeIn.close();
    }

    ifstream in;
    ofstream out;
    in.open(readFilePath, ios::binary);

//    in.open(filePath, ios::binary);

    out.open(encryptFilePath, ios::binary);

//    out.open(encryptFilePath, ios::binary);
    while (in.read((char*)&data, sizeof(data)))
    {
        divideToByte(plain, data);
        encrypt(plain, w);
        data = mergeByte(plain);
        out.write((char*)&data, sizeof(data));
        data.reset();  // 置0

    }
    in.close();
    out.close();
    qDebug() << "加密完成";

    in.open(encryptFilePath, ios::binary);
    out.open(resultFilePath, ios::binary);
    while (in.read((char*)&data, sizeof(data)))
    {
        divideToByte(plain, data);
        decrypt(plain, w);
        data = mergeByte(plain);
        out.write((char*)&data, sizeof(data));
        data.reset();  // 置0
    }
    in.close();
    out.close();

    if (isTxtFile)
    {
        QFile choseFile(QString::fromStdString(readFilePath));
        choseFile.resize(choseFile.size() - QString::fromStdString(addNumber).size());
        choseFile.close();
    }

    qDebug() << " 解密完成";
}

void Calculator::encryptChoose(QString filePath, QString passwordKey)
{

    string KeyStr = passwordKey.toStdString();
    bytes key[16];
    charToByte(key, KeyStr.c_str());
    word w[4 * (N_round + 1)];
    KeyExpansion(key, w);

    bitset<128> data;
    bytes plain[16];

    string readFilePath = filePath.toStdString();
    string str = readFilePath;
    string encryptFilePath = str.replace(str.find(".") + 1, str.size(), "stl");

    QFileInfo fileInfo(QString::fromStdString(readFilePath));

    string addNumber = "";
    bool isTxtFile = (fileInfo.suffix() == "txt");
    if (true)
    {
        ifstream judgeSizeIn(readFilePath, ios::binary);
        stringstream buffer;
        buffer << judgeSizeIn.rdbuf();
        string s = buffer.str();
        int jLength = s.size() % 16;
        if (jLength != 0)
        {
            int number;
            if ((16 - jLength) > 9)
            {
                number = 2;
            }
            else
            {
                number = 1;
            }
            for (int i = 0; i < (16 -jLength - number); i++)
            {
                addNumber += "0";
            }
            string j_Str = to_string(16 - jLength);
            addNumber += j_Str;
            ofstream judgeSizeOut(readFilePath, ios::binary | ios::app);
            judgeSizeOut << addNumber;
            judgeSizeOut.close();
        }
        judgeSizeIn.close();
    }

    QTime time;
    time.start();

    ifstream in;
    ofstream out;
    in.open(readFilePath, ios::binary);
    out.open(encryptFilePath, ios::binary);
    while (in.read((char*)&data, sizeof(data)))
    {
        divideToByte(plain, data);
        encrypt(plain, w);
        data = mergeByte(plain);
        out.write((char*)&data, sizeof(data));
        data.reset();  // 置0

    }
    in.close();
    out.close();
    qDebug() << "加密完成";
    qDebug() << time.elapsed()/1000.0 << "s";
    time.restart();

//    in.open(encryptFilePath, ios::binary);
//    out.open("/home/lbw/Desktop/png/result.zip", ios::binary);
//    while (in.read((char*)&data, sizeof(data)))
//    {
//        divideToByte(plain, data);
//        decrypt(plain, w);
//        data = mergeByte(plain);
//        out.write((char*)&data, sizeof(data));
//        data.reset();  // 置0
//    }
//    in.close();
//    out.close();

//    if (isTxtFile)
//    {
//        QFile choseFile(QString::fromStdString(readFilePath));
//        choseFile.resize(choseFile.size() - QString::fromStdString(addNumber).size());
//        choseFile.close();
//    }

//    qDebug() << " 解密完成";
//    qDebug() << time.elapsed()/1000.0 << "s";
}
