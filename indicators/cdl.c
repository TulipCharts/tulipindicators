//
// Created by dewe on 3/3/23.
//
#include "stdlib.h"
#include "../indicators.h"
#include "ta_func.h"
#include "cdl.h"


int ti_cdl_start(TI_REAL const *options) {
    return 0;
}


#define GET_SIGNALS(...) \
    int begIdx; \
    int outNBElement;                      \
    int* result = (int*)calloc(size, sizeof(int)); \
    int ta_result = \
        cdlSignature( \
            0, \
            size - 1, \
            open, \
            high, \
            low,\
            __VA_ARGS__,          \
            &begIdx, \
            &outNBElement, \
            result);  \
    assert(ta_result == TA_SUCCESS);     \

int complete(int* result, double* output, int size)
{
    for (int i = 0; i < size; i++)
    {
        output[i] = (double)( (double)result[i] / 100.0 );
    }
    free(result);
    return TI_OKAY;
}
int ti_cdl2(int size,
           TI_REAL const *const *inputs,
           TI_REAL const *options,
            TI_REAL *const *outputs)
{
    const TI_REAL* high = inputs[0];
    const TI_REAL* low = inputs[1];
    const TI_REAL* close = inputs[2];
    const TI_REAL* open = inputs[3];

    const enum CDL_TYPE patternType = (enum CDL_TYPE)options[0];
    double penetration = options[1];

    if (size <= ti_cdl_start(options))
        return TI_OKAY;

    TI_REAL* output = outputs[0];

    TA_RetCode (*cdlSignature)(
        int,
        int,
        const double[],
        const double[],
        const double[],
        const double[],
        double,
        int*,
        int*,
        int[]);

    switch (patternType)
    {
        case Mathold:
            cdlSignature = TA_CDLMATHOLD;
            break;
        case MorningDojiStar:
            cdlSignature = TA_CDLMORNINGDOJISTAR;
            break;
        case MorningStar:
            cdlSignature = TA_CDLMORNINGSTAR;
            break;
        case DarkCloudCover:
            cdlSignature = TA_CDLDARKCLOUDCOVER;
            break;
        case EveningDojiStar:
            cdlSignature = TA_CDLEVENINGDOJISTAR;
            break;
        case EveningStar:
            cdlSignature = TA_CDLEVENINGSTAR;
            break;
        case AbandonedBaby:
            cdlSignature = TA_CDLABANDONEDBABY;
            break;
        default:
            printf(
                "ti_cdl2 only supports CDL TI With Penetration Option, use ti_cdl");
            return TA_INVALID_PARAM_HOLDER;
    }

    GET_SIGNALS(close, penetration);
    return complete(result, output, size);
}


int ti_cdl(int size,
           TI_REAL const *const *inputs,
           TI_REAL const *options,
           TI_REAL *const *outputs)
{
    const TI_REAL* high = inputs[0];
    const TI_REAL* low = inputs[1];
    const TI_REAL* close = inputs[2];
    const TI_REAL* open = inputs[3];

    const enum CDL_TYPE patternType = (enum CDL_TYPE)options[0];
    if (size <= ti_cdl_start(options))
        return TI_OKAY;

    TI_REAL* output = outputs[0];

    TA_RetCode (*cdlSignature)(
        int,
        int,
        const double[],
        const double[],
        const double[],
        const double[],
        int*,
        int*,
        int[]);

    switch (patternType)
    {
        case Crows:
            cdlSignature = TA_CDL2CROWS;
            break;
        case BlackCrows:
            cdlSignature = TA_CDL3BLACKCROWS;
            break;
        case Inside:
            cdlSignature = TA_CDL3INSIDE;
            break;
        case LineStrike:
            cdlSignature = TA_CDL3LINESTRIKE;
            break;
        case ConcealBabySwall:
            cdlSignature = TA_CDLCONCEALBABYSWALL;
            break;
        case Outside:
            cdlSignature = TA_CDL3OUTSIDE;
            break;
        case StarsInSouth:
            cdlSignature = TA_CDL3STARSINSOUTH;
            break;
        case WhiteSoldiers:
            cdlSignature = TA_CDL3WHITESOLDIERS;
            break;
        case AdvanceBlock:
            cdlSignature = TA_CDLADVANCEBLOCK;
            break;
        case Belthold:
            cdlSignature = TA_CDLBELTHOLD;
            break;
        case Breakaway:
            cdlSignature = TA_CDLBREAKAWAY;
            break;
        case ClosingMarubozu:
            cdlSignature = TA_CDLCLOSINGMARUBOZU;
            break;
        case Doji:
            cdlSignature = TA_CDLDOJI;
            break;
        case DojiStar:
            cdlSignature = TA_CDLDOJISTAR;
            break;
        case DragonFlyDoji:
            cdlSignature = TA_CDLDRAGONFLYDOJI;
            break;
        case Engulfing:
            cdlSignature = TA_CDLENGULFING;
            break;
        case GapSideSideWhite:
            cdlSignature = TA_CDLGAPSIDESIDEWHITE;
            break;
        case GraveStoneDoji:
            cdlSignature = TA_CDLGRAVESTONEDOJI;
            break;
        case Hammer:
            cdlSignature = TA_CDLHAMMER;
            break;
        case HangingMan:
            cdlSignature = TA_CDLHANGINGMAN;
            break;
        case Harami:
            cdlSignature = TA_CDLHARAMI;
            break;
        case HaramiCross:
            cdlSignature = TA_CDLHARAMICROSS;
            break;
        case HighWave:
            cdlSignature = TA_CDLHIGHWAVE;
            break;
        case Hikkake:
            cdlSignature = TA_CDLHIKKAKE;
            break;
        case HikkakeMod:
            cdlSignature = TA_CDLHIKKAKEMOD;
            break;
        case HomingPigeon:
            cdlSignature = TA_CDLHOMINGPIGEON;
            break;
        case Identical3Crows:
            cdlSignature = TA_CDLIDENTICAL3CROWS;
            break;
        case InNeck:
            cdlSignature = TA_CDLINNECK;
            break;
        case InvertedHammer:
            cdlSignature = TA_CDLINVERTEDHAMMER;
            break;
        case Kicking:
            cdlSignature = TA_CDLKICKING;
            break;
        case KickingByLength:
            cdlSignature = TA_CDLKICKINGBYLENGTH;
            break;
        case LadderBottom:
            cdlSignature = TA_CDLLADDERBOTTOM;
            break;
        case LongLeggedDoji:
            cdlSignature = TA_CDLLONGLEGGEDDOJI;
            break;
        case LongLine:
            cdlSignature = TA_CDLLONGLINE;
            break;
        case Marubozu:
            cdlSignature = TA_CDLMARUBOZU;
            break;
        case MatchingGlow:
            cdlSignature = TA_CDLMATCHINGLOW;
            break;
        case OnNeck:
            cdlSignature = TA_CDLONNECK;
            break;
        case Piercing:
            cdlSignature = TA_CDLPIERCING;
            break;
        case RickShawman:
            cdlSignature = TA_CDLRICKSHAWMAN;
            break;
        case RiseFall3Methods:
            cdlSignature = TA_CDLRISEFALL3METHODS;
            break;
        case SeparatingLines:
            cdlSignature = TA_CDLSEPARATINGLINES;
            break;
        case ShootingStar:
            cdlSignature = TA_CDLSHOOTINGSTAR;
            break;
        case ShortLine:
            cdlSignature = TA_CDLSHORTLINE;
            break;
        case SpinningTop:
            cdlSignature = TA_CDLSPINNINGTOP;
            break;
        case StalledPattern:
            cdlSignature = TA_CDLSTALLEDPATTERN;
            break;
        case StickSandwich:
            cdlSignature = TA_CDLSTICKSANDWICH;
            break;
        case Takuri:
            cdlSignature = TA_CDLTAKURI;
            break;
        case TasukiGap:
            cdlSignature = TA_CDLTASUKIGAP;
            break;
        case TriStar:
            cdlSignature = TA_CDLTRISTAR;
            break;
        case Unique3River:
            cdlSignature = TA_CDLUNIQUE3RIVER;
            break;
        case UpsideGap2Crows:
            cdlSignature = TA_CDLUPSIDEGAP2CROWS;
            break;
        case XSideGap3Methods:
            cdlSignature = TA_CDLXSIDEGAP3METHODS;
            break;
        default:
            printf(
                "ti_cdl only supports candlestick patterns with no penetration option, use ti_cdl2.");
            return TA_INVALID_PARAM_HOLDER;
    }
    GET_SIGNALS(close);
    return complete(result, output, size);
}