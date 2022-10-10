// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <iostream>

int TBitField::getNumbersBitInTELEM() const
{
    return sizeof(TELEM) * 8;
}

TBitField::TBitField(int len)
{
    if (len < 0)
        throw length_error("invalid size.");


    const int cntBit = getNumbersBitInTELEM();

    BitLen = len;
    MemLen = (BitLen + cntBit - 1) / cntBit;
    pMem = new TELEM[MemLen];
    memset(pMem, 0, sizeof(TELEM) * MemLen);
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{

    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    if(pMem) delete [] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n / getNumbersBitInTELEM();
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    TELEM unit = 1;
    return (unit << (n % getNumbersBitInTELEM()));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("invalid index");

    int i = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[i] |= (mask);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("invalid index");

    int i = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[i] &= (~mask);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("invalid index");

    int i = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    return (pMem[i] & mask) >> (n % getNumbersBitInTELEM());
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf)
        return *this;

    BitLen = bf.BitLen;
    MemLen = bf.MemLen;

    if (pMem) delete[] pMem;
    pMem = new TELEM[MemLen];

    for (size_t i = 0; i < MemLen; i++)
    {
        pMem[i] = bf.pMem[i];
    }

    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return 0;

    for (size_t i = 0; i < MemLen; i++)
    {
        if (pMem[i] != bf.pMem[i])
            return 0;
    }

    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return 1 - (*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int maxLen = max(BitLen, bf.BitLen);
    int minLen = min(BitLen, bf.BitLen);
    const int cntBit = getNumbersBitInTELEM();
    int minMemLen = (minLen + cntBit - 1) / cntBit;

    TBitField ans((BitLen == maxLen? *this: bf));

    for (size_t i = 0; i < minMemLen; i++)
    {
        ans.pMem[i] = pMem[i] | bf.pMem[i];
    }
    return ans;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int maxLen = max(BitLen, bf.BitLen);
    int minLen = min(BitLen, bf.BitLen);
    const int cntBit = getNumbersBitInTELEM();
    int minMemLen = (minLen + cntBit - 1) / cntBit;

    TBitField ans(maxLen);

    for (size_t i = 0; i < minMemLen; i++)
    {
        ans.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return ans;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField ans(BitLen);
    int i = 0;
    const int cntBit = getNumbersBitInTELEM();
    while (i < BitLen)
    {
        if (i + cntBit <= BitLen)
        {
            ans.pMem[i / cntBit] = ~pMem[i / cntBit];
            i += cntBit;
        }
        else
        {
            int invert_bit = 1 - GetBit(i);

            if (invert_bit) ans.SetBit(i);
            else ans.ClrBit(i);

            i++;
        }
    }
    return ans;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    string str;
    istr >> str;
    for (int i = 0; i < bf.BitLen; i++)
    {
        if(str[i] - '0')
            bf.SetBit(i);
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++)
    {
        ostr << bf.GetBit(i);
    }
    return ostr;
}
