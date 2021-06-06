#include"libOne.h"
struct CHARA {
    float px, py, vx, vy, advSp=12, ang=0, angSp=0.05f;
    int hp = 0;
    int blinkCnt = 0;
};

void gmain() {
    window(1920, 1080, full);
    int pImg = loadImage("assets\\player.png");
    int eImg = loadImage("assets\\enemy.png");
    int pbImg = loadImage("assets\\pBullet.png");
    int ebImg = loadImage("assets\\eBullet.png");
    struct CHARA p;
    p.px = width/2;
    p.py = height-100;
    p.advSp = 5;
    p.ang = 0;
    p.angSp = 0.05f;
    p.hp = 900;
    const int numPb = 24;
    struct CHARA pb[numPb];
    int pbTriggerCnt = 0;
    struct CHARA e;
    const int numEb = 24;
    struct CHARA eb[numEb];
    int ebTriggerCnt = 0;


    while (notQuit) {

            //player
            if (isPress(KEY_A))p.ang -= p.angSp;
            if (isPress(KEY_D))p.ang += p.angSp;
            p.vx = sin(p.ang);
            p.vy = -cos(p.ang);
            if (isPress(KEY_W)) {
                p.px += p.vx * p.advSp;
                p.py += p.vy * p.advSp;
            }
            if (isPress(KEY_S)) {
                p.px += -p.vx * p.advSp;
                p.py += -p.vy * p.advSp;
            }
            if (p.px < 0)p.px = width;
            if (p.px > width)p.px = 0;
            if (p.py < 0)p.py = height;
            if (p.py > height)p.py = 0;
            //enemy
            e.px = width / 2 - sin(e.ang) * 420;
            e.py = height / 2 - cos(e.ang) * 420;
            e.ang += 0.001f;
            //player bullets
            if (isPress(KEY_SPACE)) {
                if (++pbTriggerCnt % 5 == 1) {
                    for (int i = 0; i < numPb; i++) {
                        if (pb[i].hp == 0) {
                            pb[i].hp = 1;
                            pb[i].px = p.px + p.vx * 110;
                            pb[i].py = p.py + p.vy * 110;
                            pb[i].vx = p.vx;
                            pb[i].vy = p.vy;
                            i = numPb;
                        }
                    }
                }
            }
            else {
                pbTriggerCnt = 0;
            }
            for (int i = 0; i < numPb; i++) {
                if (pb[i].hp > 0) {
                    pb[i].px += pb[i].vx * pb[i].advSp;
                    pb[i].py += pb[i].vy * pb[i].advSp;
                    pb[i].ang += pb[i].angSp;
                    if (pb[i].px<-30 || pb[i].px>width + 30 ||
                        pb[i].py<-30 || pb[i].py>height + 30) {
                        pb[i].hp = 0;
                    }
                }
            }
            //enemy bullets
            if (++ebTriggerCnt % 60 == 1) {
                ebTriggerCnt = 1;
                for (int i = 0; i < numEb; i++) {
                    if (eb[i].hp == 0) {
                        eb[i].hp = 1;
                        eb[i].px = e.px;
                        eb[i].py = e.py;
                        eb[i].vx = p.px - e.px;
                        eb[i].vy = p.py - e.py;
                        float l = sqrt(eb[i].vx * eb[i].vx + eb[i].vy * eb[i].vy);
                        eb[i].vx /= l;
                        eb[i].vy /= l;
                        i = numEb;
                    }
                }
            }
            for (int i = 0; i < numEb; i++) {
                if (eb[i].hp > 0) {
                    eb[i].px += eb[i].vx * eb[i].advSp;
                    eb[i].py += eb[i].vy * eb[i].advSp;
                    eb[i].ang += eb[i].angSp;
                    if (eb[i].px<-30 || eb[i].px>width + 30 ||
                        eb[i].py<-30 || eb[i].py>height + 30) {
                        eb[i].hp = 0;
                    }
                }
            }
            //
            for (int i = 0; i < numEb; i++) {
                if (eb[i].hp > 0) {
                    float x = eb[i].px - p.px;
                    float y = eb[i].py - p.py;
                    if (x * x + y * y < 2500 && p.blinkCnt <= 0) {
                        eb[i].hp = 0;
                        p.blinkCnt = 60;
                        //p.hp -= 100;
                    }
                }
            }
            for (int i = 0; i < numPb; i++) {
                if (pb[i].hp > 0) {
                    float x = pb[i].px - e.px;
                    float y = pb[i].py - e.py;
                    if (x * x + y * y < 10000 && e.blinkCnt <= 0) {
                        pb[i].hp = 0;
                        e.blinkCnt = 60;
                    }
                }
            }

        clear();
        rectMode(CENTER);
        for (int i = 0; i < numPb; i++) {
            if (pb[i].hp > 0) {
                image(pbImg, pb[i].px, pb[i].py, pb[i].ang);
            }
        }
        for (int i = 0; i < numEb; i++) {
            if (eb[i].hp > 0) {
                image(ebImg, eb[i].px, eb[i].py);
            }
        }
        if (p.blinkCnt > 0)--p.blinkCnt;
        if (p.blinkCnt / 5 % 2 == 0) {
            image(pImg, p.px, p.py, p.ang);
        }
        else {
            if (p.hp > 0)p.hp -= 5;
        }
        if (e.blinkCnt > 0)--e.blinkCnt;
        if (e.blinkCnt / 5 % 2 == 0)image(eImg, e.px, e.py);
        
        //rectMode(CORNER);
        //fill(0, 200, 0);
        //rect(50, 50, p.hp, 50);
        //if (p.hp <= 0) {
        //    textSize(200);
        //    text("GameOver", 0, 200);
        //}
    }
}
