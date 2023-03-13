#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

uint64_t *array;
uint64_t arr_size;

uint64_t itr_bin, itr_qtr, itr_inter;

int generate_data(){
    printf("\nGenerating %lu elements...\n", arr_size);
    
    array = (uint64_t*)malloc(arr_size * sizeof(uint64_t));

    if(array == NULL){
        printf("\nFailed to allocate memory");
        exit(1);
    }

    uint64_t num = 0, i;
    for(i = 0; i < arr_size; i++){
        num += rand() % 10 + 1;
        array[i] = num;
    }
    
    return i;
}

int binary(uint64_t key){

    // Binary search
    
    uint64_t start   = 0;
    uint64_t end     = arr_size - 1;
    uint64_t mid     = 0;

    while(start <= end){
        itr_bin++;
        mid = (uint64_t)((start + end)/2);

        if (array[mid] == key){
            return mid;
        }

        else if(array[mid] > key){
            end = mid - 1;
        }

        else if(array[mid] < key){
            start = mid+1;
        }
    }

    return -1;
}

int interpolationSearch(uint64_t x)
{
    // Find indexes of two corners
    uint64_t lo = 0, hi = (arr_size - 1);
 
    // Since array is sorted, an element present
    // in array must be in range defined by corner
    while (lo <= hi && x >= array[lo] && x <= array[hi])
    {
        itr_inter++;
        if (lo == hi)
        {
            if (array[lo] == x) return lo;
            return -1;
        }
        // Probing the position with keeping
        // uniform distribution in mind.
        int pos = lo + (((double)(hi - lo) /
            (array[hi] - array[lo])) * (x - array[lo]));
 
        // Condition of target found
        if (array[pos] == x)
            return pos;
 
        // If x is larger, x is in upper part
        if (array[pos] < x)
            lo = pos + 1;
 
        // If x is smaller, x is in the lower part
        else
            hi = pos - 1;
    }
    return -1;
}

int quaternary_search(uint64_t key){
    uint64_t start = 0;
    uint64_t end = arr_size - 1;
    uint64_t gap;

    uint64_t p, q, r;

    while(start <= end){
        // COUNTING ITERATIONS
        itr_qtr++;
        gap = (end - start)/4;

        // CALCULATING MID POINTS
        p = start + gap;
        q = p + gap;
        r = q + gap;

        // CHECKING ALL MID POINTS
        if(array[start] == key) return start;
        else if(array[end] == key) return end;
        else if(array[p] == key) return p;
        else if(array[q] == key) return q;
        else if(array[r] == key) return r;

        // START AND END ARE ADJECENT, SO ELEMENT NOT IN ARRAY

        if(key > array[r]){             // KEY IS IN RANGE 1
            start = r + 1;
        }
        else if(key > array[q]){        // KEY IS IN RANGE 2
            start = q + 1;
            end = r - 1;
        }
        else if(key > array[p]){        // KEY IS IN RANGE 3
            start = p + 1;
            end = q - 1;
        }
        else{                           // KEY IS IN RANGE 4
            end = p - 1;
        }
    }

    // ELEMENT NOT FOUND
    return -1;
}

int main(int argc, char **argv){

    if(argc < 3){
        printf("Expected 2 arguments!\n");
        exit(0);
    }

    // CONVERTING COMMAND LINE ARGUMENTS TO INTEGER
    arr_size = atoi(argv[1]);
    int pass = atoi(argv[2]);

    generate_data();

    clock_t s, e;
    float binary_time[40];
    float quaternary_time[40];
    float interpol_time[40];

    int search_range = arr_size;

    printf("\nRunning search algorithms for %lu elements\n\n", arr_size);
    printf("\033[33;3mSr     Binary\t\tQuaternary\tInterpolation\033[0m\n");

    // RUNNING SEARCH
    for(int j = 0; j < pass; j++){
        s = clock();
        for(int i = 0; i < search_range; i++){
            int val = binary(array[i]);
            if(val == -1) printf("element not found - binary\n");
        }
        e = clock();
        // Storing time in array to find avg at end
        binary_time[j] = (double)(e - s)/CLOCKS_PER_SEC;

        s = clock();
        for(int i = 0; i < search_range; i++){
            int val = quaternary_search(array[i]);
            if(val == -1) printf("element not found - new\n");
        }
        e = clock();
        quaternary_time[j] = (double)(e - s)/CLOCKS_PER_SEC;

        s = clock();
        for(int i = 0; i < search_range; i++){
            int val = interpolationSearch(array[i]);
            if(val == -1) printf("element not found - quad\n");
        }
        e = clock();
        interpol_time[j] = (double)(e - s)/CLOCKS_PER_SEC;

        // PRINTING RUNTIME OF EACH PASS
        printf("%-6d %-17.6f%-16.6f%-17.6f\n", j, binary_time[j], quaternary_time[j], interpol_time[j]);
    }

    float binary_total_time = 0, quaternary_total_time = 0, interpol_total_time = 0;

    for(int i = 0; i < pass; i++){
        binary_total_time += binary_time[i];
        quaternary_total_time += quaternary_time[i];
        interpol_total_time += interpol_time[i];
    }
    
    // CALCULATING AVERAGE OF ALL PASSES
    binary_total_time     = binary_total_time / pass;
    quaternary_total_time = quaternary_total_time / pass;
    interpol_total_time   = interpol_total_time / pass;


    // TIME DELTAS IN PERCENTAGE
    float delta_quad_bin        = (( binary_total_time     - quaternary_total_time) *100) / binary_total_time;
    float delta_interpol_bin    = (( binary_total_time     - interpol_total_time)   *100) / binary_total_time;
    float delta_quad_interpol   = (( quaternary_total_time - interpol_total_time)   *100) / quaternary_total_time;

    // PRINTING FORMATTED OUTPUT
    printf("\n\n\e[4;33mAverage times\033[0m\n");
    printf("Binary         %fs\n", binary_total_time);
    printf("Quaternary     %fs (\e[1;32m%.2f%c\e[0m faster than Binary)\n", quaternary_total_time, delta_quad_bin, 37);
    printf("Interpolation  %fs (\e[1;32m%.2f%c\e[0m faster than Binary) (\e[1;32m%.2f%c\e[0m faster than quaternary)\n", interpol_total_time, delta_interpol_bin, 37, delta_quad_interpol, 37);
    printf("\n\e[4;33mTotal iterations\033[0m\n");
    printf("Binary         %lu\n", itr_bin);
    printf("Quaternary     %lu (\e[1;32m%.2f%c\e[0m less iterations than Binary)\n", itr_qtr, (double)((itr_bin - itr_qtr)*100)/itr_bin, 37);
    printf("Interpolation  %lu (\e[1;32m%.2f%c\e[0m less iterations than Binary) (\e[1;32m%.2f%c\e[0m less iterations than Quaternary)\n\n", itr_inter, (double)((itr_bin - itr_inter)*100)/itr_bin, 37, (double)((itr_qtr - itr_inter)*100)/itr_qtr, 37);

    // STORING DATA TO FILE
    FILE *f = fopen("data.txt", "a");
    fprintf(f, "%lu, %f, %f, %f\n", arr_size, binary_total_time, quaternary_total_time, delta_quad_bin);
    fclose(f);

    return 0;
}
