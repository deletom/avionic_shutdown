/*
 * Extinction du système
 * Auteur: Thomas DELECOURT
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <time.h>

#include "hiredis/hiredis.h"

// Paramétres de la matrice LED
#define FILEPATH "/dev/fb1"
#define NUM_WORDS 64
#define FILESIZE (NUM_WORDS * sizeof(uint16_t))

// Définition de la LED de base
#define LED_BASE 56

int main(void) {

    // Variables utilisées pour la matrice LED
    uint16_t *map;
    struct fb_fix_screeninfo fix_info;
    int FrameBufferMatrix;

    // Ouverture du buffer
    FrameBufferMatrix = open(FILEPATH, O_RDWR);
    if (FrameBufferMatrix == -1) {
        perror("Erreur : Impossible d ouvrir FILEPATH");
        exit(EXIT_FAILURE);
    }

    // Information de l'ecran
    if (ioctl(FrameBufferMatrix, FBIOGET_FSCREENINFO, &fix_info) == -1) {
        perror("Erreur : Informations Ecran");
        close(FrameBufferMatrix);
        exit(EXIT_FAILURE);
    }

    // Nous sommes bien sur l'ecran du SenseHat
    if (strcmp(fix_info.id, "RPi-Sense FB") != 0) {
        printf("%s\n", "RPi-Sense FB non trouve");
        close(FrameBufferMatrix);
        exit(EXIT_FAILURE);
    }

    map = mmap(NULL, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, FrameBufferMatrix, 0);
    if (map == MAP_FAILED) {
        close(FrameBufferMatrix);
        perror("Error mmapping the file");
        exit(EXIT_FAILURE);
    }

    // On éteint la matrice le LED
    memset(map, 0, FILESIZE);
    close(FrameBufferMatrix);

    return 0;
}
