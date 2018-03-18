#include "kor.h"
#include "stdio.h"

void kor_parce_ini (Kor* kor) {
    FILE* korIniFile = fopen("kor.ini", "r");

    fscanf(korIniFile, "%s\n", kor->folderName);
    fscanf(korIniFile, "%d\n", &(kor->startVideoTime));
    fscanf(korIniFile, "%d\n", &(kor->firstPointTime));
    fscanf(korIniFile, "%lf\n", &(kor->lat));
    fscanf(korIniFile, "%lf\n", &(kor->lon));
    fscanf(korIniFile, "%lf\n", &(kor->alt));
    fscanf(korIniFile, "%lf\n", &(kor->yaw));
    fscanf(korIniFile, "%f\n", &(kor->duration));
    fscanf(korIniFile, "%f\n", &(kor->framePerSeconds));
    fscanf(korIniFile, "%lf\n", &(kor->calibLat));
    fscanf(korIniFile, "%lf\n", &(kor->calibLon));

    fclose(korIniFile);
}

void kor_dump(Kor* kor) {
    printf("kor.ini: \n");
    printf("name %s\n", kor->folderName);
    printf("start video time %d\n", kor->startVideoTime);
    printf("firstPointTime %d\n", kor->firstPointTime);
    printf("lat %lf\n", kor->lat);
    printf("lon %lf\n", kor->lon);
    printf("alt %lf\n", kor->alt);
    printf("yaw %lf\n", kor->yaw);
    printf("duration %f\n", kor->duration);
    printf("frame rate %f\n", kor->framePerSeconds);
    printf("calib Lat %lf\n", kor->calibLat);
    printf("calib Lon %lf\n", kor->calibLon);
    printf("--------------------------\n");
}
