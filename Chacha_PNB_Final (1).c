#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define ul unsigned int
#define ull unsigned long long

#define rotateleft(x, n) (((x) << (n)) ^ ((x) >> (32 - n)))
#define rotateright(x, n) (((x) >> (n)) ^ ((x) << (32 - n)))

#define update(a, b, n) ((rotateleft((a) ^ (b), (n))))
#define myrand32 ((ul)(4294967296.0 * drand48()))

ull MOD = 4294967296;

void initializeR(ul *x)
{
        int i;
        for (i = 0; i < 16; i++)
        {
                x[i] = myrand32;
        }
        x[0] = 0x61707865;
        x[1] = 0x3320646e;
        x[2] = 0x79622d32;
        x[3] = 0x6b206574;
}

void copystate(ul *x1, ul *x)
{
        int i;
        for (i = 0; i < 16; i++)
        {
                x1[i] = x[i];
        }
}

void qr(ul *x0, ul *x1, ul *x2, ul *x3)
{
        ul z0, z1, z2, z3;

        z0 = *x0 + *x1;
        z3 = update(*x3, z0, 16);
        z2 = *x2 + z3;
        z1 = update(*x1, z2, 12);
        z0 = z0 + z1;
        z3 = update(z3, z0, 8);
        z2 = z2 + z3;
        z1 = update(z1, z2, 7);

        *x0 = z0, *x1 = z1, *x2 = z2, *x3 = z3;
}

void fhalfqr(ul *x0, ul *x1, ul *x2, ul *x3)
{
        ul p, q, r, s;

        p = *x0;
        q = *x1;
        r = *x2;
        s = *x3;

        p += q;
        s ^= p;
        s = rotateleft(s, 16);
        r += s;
        q ^= r;
        q = rotateleft(q, 12);
        // p ^= q; s ^= p; s = rotateleft(s,25);
        // r ^= s; q ^= r; q = rotateleft(q,11);

        *x0 = p;
        *x1 = q;
        *x2 = r;
        *x3 = s;
}

void shalfqr(ul *x0, ul *x1, ul *x2, ul *x3)
{
        ul p, q, r, s;

        p = *x0;
        q = *x1;
        r = *x2;
        s = *x3;

        // p ^= q; s ^= p; s = rotateleft(s,19);
        // r ^= s; q ^= r; q = rotateleft(q,17);
        p += q;
        s ^= p;
        s = rotateleft(s, 8);
        r += s;
        q ^= r;
        q = rotateleft(q, 7);

        *x0 = p;
        *x1 = q;
        *x2 = r;
        *x3 = s;
}

void sinqr(ul *x0, ul *x1, ul *x2, ul *x3)
{
        ul p, q, r, s;

        p = *x0, q = *x1, r = *x2, s = *x3;

        q = rotateright(q, 7);
        q ^= r;
        r -= s;
        s = rotateright(s, 8);
        s ^= p;
        p -= q;
        // q = rotateright(q, 17);
        // q ^= r;
        // r -= s;
        // s = rotateright(s, 19);
        // s ^= p;
        // p -= q;

        *x0 = p, *x1 = q, *x2 = r, *x3 = s;
}

void finqr(ul *x0, ul *x1, ul *x2, ul *x3)
{
        ul p, q, r, s;

        p = *x0, q = *x1, r = *x2, s = *x3;

        // q = rotateright(q, 11);
        // q ^= r;
        // r -= s;
        // s = rotateright(s, 25);
        // s ^= p;
        // p -= q;
        q = rotateright(q, 12);
        q ^= r;
        r -= s;
        s = rotateright(s, 16);
        s ^= p;
        p -= q;

        *x0 = p, *x1 = q, *x2 = r, *x3 = s;
}

void roundodd(ul *x)
{
        qr(&(x[0]), &(x[4]), &(x[8]), &(x[12]));
        qr(&(x[1]), &(x[5]), &(x[9]), &(x[13]));
        qr(&(x[2]), &(x[6]), &(x[10]), &(x[14]));
        qr(&(x[3]), &(x[7]), &(x[11]), &(x[15]));
}

void roundeven(ul *x)
{
        qr(&(x[0]), &(x[5]), &(x[10]), &(x[15]));
        qr(&(x[1]), &(x[6]), &(x[11]), &(x[12]));
        qr(&(x[2]), &(x[7]), &(x[8]), &(x[13]));
        qr(&(x[3]), &(x[4]), &(x[9]), &(x[14]));
}

void fhalfroundeven(ul *x)
{

        fhalfqr(&(x[0]), &(x[5]), &(x[10]), &(x[15]));
        fhalfqr(&(x[1]), &(x[6]), &(x[11]), &(x[12]));
        fhalfqr(&(x[2]), &(x[7]), &(x[8]), &(x[13]));
        fhalfqr(&(x[3]), &(x[4]), &(x[9]), &(x[14]));
        // transpose(x);
}

void shalfroundeven(ul *x)
{

        shalfqr(&(x[0]), &(x[5]), &(x[10]), &(x[15]));
        shalfqr(&(x[1]), &(x[6]), &(x[11]), &(x[12]));
        shalfqr(&(x[2]), &(x[7]), &(x[8]), &(x[13]));
        shalfqr(&(x[3]), &(x[4]), &(x[9]), &(x[14]));
        // transpose(x);
}

void inqr(ul *x0, ul *x1, ul *x2, ul *x3)
{
        ul p, q, r, s;

        p = *x0, q = *x1, r = *x2, s = *x3;

        q = rotateright(q, 7);
        q ^= r;
        r -= s;
        s = rotateright(s, 8);
        s ^= p;
        p -= q;
        q = rotateright(q, 12);
        q ^= r;
        r -= s;
        s = rotateright(s, 16);
        s ^= p;
        p -= q;

        *x0 = p, *x1 = q, *x2 = r, *x3 = s;
}

void inroundo(ul *x)
{
        inqr(&(x[3]), &(x[7]), &(x[11]), &(x[15]));
        inqr(&(x[2]), &(x[6]), &(x[10]), &(x[14]));
        inqr(&(x[1]), &(x[5]), &(x[9]), &(x[13]));
        inqr(&(x[0]), &(x[4]), &(x[8]), &(x[12]));
}

void inrounde(ul *x)
{
        inqr(&(x[3]), &(x[4]), &(x[9]), &(x[14]));
        inqr(&(x[2]), &(x[7]), &(x[8]), &(x[13]));
        inqr(&(x[1]), &(x[6]), &(x[11]), &(x[12]));
        inqr(&(x[0]), &(x[5]), &(x[10]), &(x[15]));
}

void sinrounde(ul *x)
{
        sinqr(&(x[3]), &(x[4]), &(x[9]), &(x[14]));
        sinqr(&(x[2]), &(x[7]), &(x[8]), &(x[13]));
        sinqr(&(x[1]), &(x[6]), &(x[11]), &(x[12]));
        sinqr(&(x[0]), &(x[5]), &(x[10]), &(x[15]));
}

int main()
{
        ul x[16], x1[16], x0[16], x01[16], z1[16], z2[16], diff, diff1, pattern, pt;
        int i, j, j3, j4,

            A1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 19, 20, 21, 22, 31, 32, 33, 34, 35, 36, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 71, 72, 73, 74, 77, 78, 79, 80, 83, 84, 85, 86, 89, 90, 91, 95, 99, 100, 103, 104, 105, 106, 107, 108, 109, 110, 111, 123, 124, 125, 126, 127, 128, 129, 130, 140, 141, 142, 143, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 198, 199, 200, 201, 204, 205, 206, 207, 210, 211, 212, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 231, 232, 244, 245, 246, 247, 248},//# 162



           

            A2[] = {10, 22, 36, 68, 74, 80, 86, 91, 100, 111, 130, 143, 194, 201, 207, 212, 228, 232, 248}, //#19

            A3[]={14, 26,  115, 147,  255}; //# 5

        ul u1,u2,u3;

        int LL = 163, LL2 = 19 ; //19;

        ull loop = 0;
        double cnt = 0;

        srand48(time(NULL));

        while (loop < 1024 * 1024 * 1024)
        {

                pt = (0x00000001 << 9) ^ (0x00000001 << 29);
                initializeR(x);
                copystate(x1, x);
                x1[15] = x[15] ^ pt;
                copystate(x0, x);
                copystate(x01, x1);

                roundodd(x);
                roundodd(x1);
                roundeven(x);
                roundeven(x1);
                roundodd(x);
                roundodd(x1);
                roundeven(x);
                roundeven(x1);
                roundodd(x);
                roundodd(x1);

                diff = x[2] ^ x1[2] ^ (x[6] >> 7) ^ (x1[6] >> 7) ^ (x[6] >> 19) ^ (x1[6] >> 19) ^ (x[10] >> 12) ^ (x1[10] >> 12) ^ (x[14] >> 0) ^ (x1[14] >> 0);

                roundeven(x);
                roundeven(x1);
                roundodd(x);
                roundodd(x1);

                for (i = 0; i < 16; i++)
                {
                        z1[i] = (x[i] + x0[i]);
                        z2[i] = (x1[i] + x01[i]);
                }






                for (j = 0; j < 162; j++)
                {

                        j3 = (A1[j] / 32) + 4;
                        j4 = A1[j] % 32;

                        if (((x0[j3] >> j4) % 2) != 0)

                        {
                                x0[j3] = x0[j3] ^ (0x00000001 << j4);
                                x01[j3] = x01[j3] ^ (0x00000001 << j4);
                        }
                }

                for (j = 0; j < 19; j++)
                {

                        j3 = (A2[j] / 32) + 4;
                        j4 = A2[j] % 32;

                        if (((x0[j3] >> j4) % 2) == 0)

                        {
                                x0[j3] = x0[j3] ^ (0x00000001 << j4);
                                x01[j3] = x01[j3] ^ (0x00000001 << j4);
                        }
                }



                 for (j = 0; j < 5; j++)
                {

                        j3 = (A3[j] / 32) + 4;
                        j4 = A3[j] % 32;

                        if (drand48()<0.5)

                        {
                                x0[j3] = x0[j3] ^ (0x00000001 << j4);
                                x01[j3] = x01[j3] ^ (0x00000001 << j4);
                        }
                }


                
          


                for (i = 0; i < 16; i++)
                {
                        x[i] = z1[i] + (MOD - (x0[i]));
                        x1[i] = z2[i] + (MOD - (x01[i]));
                }

                inroundo(x);
                inroundo(x1);
                inrounde(x);
                inrounde(x1);
               


                
             




                pattern = 0x00000001 << 0;

                diff1 = x[2] ^ x1[2] ^ (x[6] >> 7) ^ (x1[6] >> 7) ^ (x[6] >> 19) ^ (x1[6] >> 19) ^ (x[10] >> 12) ^ (x1[10] >> 12) ^ (x[14] >> 0) ^ (x1[14] >> 0);

                if (((diff ^ diff1) & pattern) == 0)
                {
                        cnt = cnt + 1.0;
                }

                loop++;

                if (loop > 0 && loop % (1024 * 1024) == 0)
                        printf(" %llu      %0.10f   \n", loop / (1024 * 1024), 2 * ((cnt / loop) - 0.5));
        }
}

