#include<stdio.h>
#include<math.h>
float G = 6.67408e-11;

float odleglosc(float *r1, float *r2, int n){
    float r12 = 0;
    for(int i=0; i<n; i++){
        r12 += pow(r1[i] - r2[i], 2);
    }
    return sqrt(r12);
}

int sila(float *F, float M1, float *r1, float M2, float *r2, int n){
    int i;
    float Ftmp = -G * M1 * M2 / pow( odleglosc(r1,r2,n) , 3 );
    for(i = 0; i < n; i++) F[i] = Ftmp * ( r1[i] - r2[i] );
    return 0;
}

int main(){
    const float Mz = 5.97219e+24;
    const float Mk = 7.347673e+22; 
    float t0 = 0;
    float tN = 30 * 24 * 3600;
    float dt,t;

    //  WARUNKI POCZĄTKOWE

    float rz[2] = {0,0};
    float vz[2] = {0,0};

    float rk[2] = {405696e+3,0};
    float vk[2] = {0,968};

    float Fz[2] = {0,0};
    float Fk[2] = {0,0};

    sila(Fz,Mz,rz,Mk,rk,2);
    sila(Fk,Mk,rk,Mz,rz,2);

    FILE *wp = fopen("dane.txt", "w");

    dt = 3600;
    t = t0;

    fprintf(wp,"#   t   xz   yz   xk   yk   vxz  vyz   vxk   vyk   Fzx   Fzy   Fkx   Fky\n");
    fprintf(wp,"%f  %e  %e  %e  %e  %e %e %e %e %e %e %e %e\n",t,rz[0],rz[1],rk[0],rk[1],vz[0],vz[1],vk[0],vk[1],Fz[0],Fz[1],Fk[0],Fk[1]);

    while( t < tN ){
        t+=dt;
        
        rz[0] += vz[0]*dt + Fz[0]/(2*Mz) *dt*dt;
        rz[1] += vz[1]*dt + Fz[1]/(2*Mz) *dt*dt;

        rk[0] += vk[0]*dt + Fk[0]/(2*Mk) *dt*dt;
        rk[1] += vk[1]*dt + Fk[1]/(2*Mk) *dt*dt;

        float FtempZ[2] = {Fz[0],Fz[1]};
        float FtempK[2] = {Fk[0],Fk[1]};

        sila(Fz,Mz,rz,Mk,rk,2);
        sila(Fk,Mk,rk,Mz,rz,2);

        vz[0] += ( Fz[0] + FtempZ[0] ) / (2*Mz) *dt;
        vz[1] += ( Fz[1] + FtempZ[1] ) / (2*Mz) *dt;
        
        vk[0] += ( Fk[0] + FtempK[0] ) / (2*Mk) *dt;
        vk[1] += ( Fk[1] + FtempK[1] ) / (2*Mk) *dt;

        fprintf(wp,"%f  %e  %e  %e  %e  %e %e %e %e %e %e %e %e\n",t,rz[0],rz[1],rk[0],rk[1],vz[0],vz[1],vk[0],vk[1],Fz[0],Fz[1],Fk[0],Fk[1]);
    }

    fclose(wp);

    return 0;
}

//Wykres główny

//plot "dane.txt" u 2:3 t 'Z' w l, "dane.txt" u 5:6 t 'K' w l

//Tor ruchu ksiezyca wzgledem ziemi

//plot "dane.txt" u ($5-$2) t 'Z' w l, "dane.txt" u ($6-$3) t 'K' w l