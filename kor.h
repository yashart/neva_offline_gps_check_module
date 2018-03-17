#ifndef KOR_H
#define KOR_H

struct Kor {
    char folderName[100];
    int startVideoTime;
    int firstPointTime;
    double lat;
    double lon;
    double alt;
    double yaw;
    float duration;
    float framePerSeconds;
};

void kor_parce_ini (Kor* kor);
void kor_dump(Kor* kor);

#endif
