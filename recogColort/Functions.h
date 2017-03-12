//
//  Functions.h
//  recogColort
//
//  Created by Алексадр Тюльпанов on 04.03.17.
//  Copyright © 2017 Алексадр Тюльпанов. All rights reserved.
//

#ifndef Functions_h
#define Functions_h

#include <vector>
void reworkPoint(int comp[][], vector<int> compSize) {
    for (int x = 0; x < w; x++) {
        for (int y = 0;y < h; y++) {
            if (comp[y][x] == -1) {
                comp[y][x] = compSize.size();
                compSize.push_back(1);
                st.push_back(cvPoint(x, y));
                while (st.size() > 0) {
                    CvPoint p = st.back();
                    st.pop_back();
                    int dx[] = {0,0,1,-1};
                    int yd[] = {1,-1,0,0};
                    for (int i = 0; i < 4; i++) {
                        CvPoint np = cvPoint(x+dx[i], y + dx[i]);
                        if (np.x < 0 || np.y < 0 || np.x >= w || np.y >= h) continue;
                        if (PointVal(gray, np.x, np.y, 0) || comp[np.y][np.x] != -1) continue;
                            comp[np.y][np.x] = compSize.size() - 1;
                            compSize[compSize.size() - 1]++;
                    }
                }
            }
        }
    }
}

#endif /* Functions_h */
