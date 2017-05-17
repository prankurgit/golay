#include "golay.h"

void DefineOffSetLength(struct Item *it1)
/*
 * Function to get the 'offset_begin' and 'length' as user input
 *
 * Inputs:
 * item = pointer to the struct to store the necessary informations
 *
 * offset_begin = the amount of bytes (in decimal) that will be script from the beginning
 * Length = the mount of bytes that will be used (in decimal)
 */
{
    char oSet[12];
    //char iLength[12];
    char *p;
    unsigned int ch, i = 0;
    unsigned int error = 0;

    while(true){
		cout << "*******************************************************************************" << endl;
		cout << "*                                                                             *" << endl;
		cout << "*                  Define the 'offsets' for the PUF-Response:                 *" << endl;
		cout << "*                                                                             *" << endl;
		cout << "*                   0  = use complete binary PUF-Response                     *" << endl;
		cout << "*                   1  = the first byte will be skipped                       *" << endl;
		cout << "*                  'x' = 'x' bytes from both ends of file will be skipped     *" << endl;
		cout << "*                                                                             *" << endl;
		cout << "*******************************************************************************" << endl;

		if(error) ErrorMessages(error, i);
		error = 0;

		cout << endl << "Type in the offset from beginning of file in bytes (as decimal number): ";
		if (fgets(oSet, sizeof(oSet), stdin)) {
			/* fgets succeeds, scan for newline character */
			p = strchr(oSet, '\n');
				if (p) {
					*p = '\0';

					//check input if only digits are used
					for(i = 0; i < sizeof(oSet)-1; i++){
						if(oSet[i] != '\0' && !isdigit(oSet[i])){
						   error = 1;
						   break;
						}
						 if(oSet[i] == '\0') i = sizeof(oSet);
					}
					if(error == 0){

						// Set the offset_begin
						it1->offset_begin = atol(oSet);

						}
					}
					else {
					/* newline not found, flush stdin to end of line */
						while (((ch = getchar()) != '\n')
							&& !feof(stdin)
							&& !ferror(stdin)
						);
						error = 2;
					}
		}
		else {
			/* fgets failed, handle error */
			cin.clear();
			error = 3;
		}
        cout << endl << "Type in the offset from end of the file in bytes (as decimal number): ";
        if (fgets(oSet, sizeof(oSet), stdin)) {
            /*fgets succeds scan for newline character*/
            p = strchr(oSet, '\n');
            if (p) {
                *p = '\0';

                //check input if only digits are allowed
                for (i = 0; i < sizeof(oSet) - 1; i++) {
                    if (oSet[i] != '\0' && !isdigit(oSet[i])) {
                        error = 1;
                        break;
                    }
                    if (oSet[i] == '\0') i = sizeof(oSet);
                }
                if (error == 0)
                    it1->offset_end = atol(oSet);
            }
            else {
                /*newline char not found*/
                while (((ch = getchar()) != '\n') && !feof(stdin) &&
                        !ferror(stdin));
                error = 2;
            }
        }
        else {
            /*fgets failed, handle error*/
            cin.clear();
            error = 3;
        }
		if(!error) break;
	}
}

unsigned int SetInputLen(struct Item *item, int option)
/*
 * Function to calculate the input_length based on the offsets
 * make sure the offsets and it1->input_file_name are initialized before calling
 * this function.
 */
{
    FILE *fd;
    unsigned long filesize;
	// Open the specified file
    switch (option) {
        case 0:
            if((fd = fopen(item->input_PUF_name, "rb")) == NULL) return 12;
            break;
        case 1:
            if((fd = fopen(item->output_Key_name, "rb")) == NULL) return 12;
            break;
        case 2:
            if((fd = fopen(item->input_HD_name, "rb")) == NULL) return 12;
            break;
        default:
            printf("incorrect option!\n");
            break;
    }

    // Get the filesize
    fseek(fd, 0, SEEK_END);
    filesize = ftell(fd);
    rewind(fd);

    // Go to the offset_begin position in the file
    fseek(fd, item->offset_begin, SEEK_SET);

    //check if both offset_begin dont exceed the filesize
    do {
        if ((item->offset_begin + item->offset_end) > filesize) {
            printf("offset_begin and offset_end exceeds filesize, set them again\n");
            DefineOffSetLength(item);
        }
    } while ((item->offset_begin + item->offset_end) > filesize);
    //set the length to be read as the filesize - sum of offsets
    item->input_length = filesize - item->offset_begin - item->offset_end;
    printf("input_length : %lu\n", item->input_length);
    // Check if the chosen part of the PUF-Response is valid
    if((item->offset_begin+item->input_length) > filesize) return 13;

    fclose(fd);
    return 0;
}

long arr2int(int *a,int r)
/*
 * Convert a binary vector of Hamming weight r, and nonzero positions in
 * array a[1]...a[r], to a long integer \sum_{i=1}^r 2^{a[i]-1}.
 */
{
   int i;
   long mul, result = 0, temp;

   for (i=1; i<=r; i++) {
      mul = 1;
      temp = a[i]-1;
      while (temp--)
         mul = mul << 1;
      result += mul;
      }
   return(result);
}
//
//*****************************************************************************
//
void nextcomb(int n, int r, int *a)
/*
 * Calculate next r-combination of an n-set.
 */
{
    int  i, j;

    a[r]++;
    if (a[r] <= n)
        return;
    j = r - 1;
    while (a[j] == n - r + j)
        j--;
    for (i = r; i >= j; i--)
        a[i] = a[j] + i - j + 1;
    return;
}
//
//*****************************************************************************
//
long get_syndrome(long pattern)
/*
 * Compute the syndrome corresponding to the given pattern, i.e., the
 * remainder after dividing the pattern (when considering it as the vector
 * representation of a polynomial) by the generator polynomial, GENPOL.
 * In the program this pattern has several meanings: (1) pattern = infomation
 * bits, when constructing the encoding table; (2) pattern = error pattern,
 * when constructing the decoding table; and (3) pattern = received vector, to
 * obtain its syndrome in decoding.
 */
{
    long aux = X22;

    if (pattern >= X11)
        while (pattern & MASK12) {
            while (!(aux & pattern))
                aux = aux >> 1;
            pattern ^= (aux/X11) * GENPOL;
        }
    return(pattern);
}
//****************************** END GOLAY Functions ***********************************************
//
// ########################## START MAJORITYVOTE ##############################
// binary bitwise MajorityVoting
int majorityVoting(unsigned long pos, int fact)
{
    int c = 0;
    int zero = 0;
    int one = 0;
    unsigned long posi = pos;
    while(c < fact){
        if( ((posi >> ((fact-1)-c)) & 0x00000001) == 1){
            one++;
        } else{
            zero++;
        }
        c++;
    }
    if(zero > one) {			
        return 0;}
    else {
        return 1;}
}

int Savefile(unsigned char *rdata)
{
    FILE *fd;
    if ((fd = fopen("pkr", "wb")) == NULL)
        printf("unable to open pkr file!\n");
    fwrite(rdata, 1, 812, fd);    
    fclose(fd);

    return 0;
}
// ########################## END MAJORITYVOTE ##############################
//
//############################## START DECODING ##########################
int recoverOriginalData(volatile unsigned char *sramData, int LRfactor, int originalSecretSize, int len, char *typ)
{
    // Variables of this function
    int i = 0;
    int j = 0;
    int k = 0;
    long recd, recd2, temp;
    unsigned long pos8 = 0, pos7 = 0 ,pos6 = 0 , pos5 = 0, pos4 = 0, pos3 = 0, pos2 = 0, pos1 = 0;
    unsigned char recLR = 0x00;
    unsigned char *recoverdFromLR = (unsigned char *)malloc(sizeof(unsigned char) * len);
    unsigned char *recoverdSecret = (unsigned char *)malloc(sizeof(unsigned char) * originalSecretSize);
    unsigned char *helperData = (unsigned char *)malloc(sizeof(unsigned char) * (len*LRfactor));


    //get helper data from file
    char strin[100];

    FILE *ptr2;
    if (LRfactor == 7){
        if (typ == "pu"){
            //printf("Give the file path to file \"helperdata7pu\": ");
            //scanf("%s", strin);
            //if (strin[0] == 'C')
            //    strcat(strin,":\\Users\\Nikolas\\Desktop\\NetBeansProjects\\generate\\helperdata7pu.bin");
            //else strcat(strin,"\\helperdata7pu.bin");
            ptr2 = fopen( "/home/prankur/golay/generate/helperdata7pu.bin", "rb" );
        } else {
            //printf("Give the file path to file \"helperdata7pr\": ");
            //scanf("%s", strin);
            //if (strin[0] == 'C')
            //    strcat(strin,":\\Users\\Nikolas\\Desktop\\NetBeansProjects\\generate\\helperdata7pr.bin");
            //else strcat(strin,"\\helperdata7pr.bin");
            ptr2 = fopen( "/home/prankur/golay/generate/helperdata7pr.bin", "rb" );
        }
    }
    if (LRfactor == 15){
        if (typ == "pu"){
            //printf("Give the file path to file \"helperdata15pu\": ");
            //scanf("%s", strin);
            //if (strin[0] == 'C')
            //    strcat(strin,":\\Users\\Nikolas\\Desktop\\NetBeansProjects\\generate\\helperdata15pu.bin");
            //else strcat(strin,"\\helperdata15pu.bin");
            ptr2 = fopen( "/home/prankur/golay/generate/helperdata15pu.bin", "rb" );
        } else {
            //printf("Give the file path to file \"helperdata15pr\": ");
            //scanf("%s", strin);
            //if (strin[0] == 'C')
            //    strcat(strin,":\\Users\\Nikolas\\Desktop\\NetBeansProjects\\generate\\helperdata15pr.bin");
            //else strcat(strin,"\\helperdata15pr.bin");
            ptr2 = fopen( "/home/prankur/golay/generate/helperdata15pr.bin", "rb" );
        }
    }
    if (!ptr2)
    {
        printf("Unable to open file 2!\n");
        return 1;
    }
    fread(helperData,sizeof(unsigned char),(len*LRfactor),ptr2);


    fclose(ptr2);


    /*
     * ---------------------------------------------------------------------
     *                  Generate GOLAY DECODING TABLE
     *
     * An entry to the decoding table is a syndrome and the resulting value
     * is the most likely error pattern. First an error pattern is generated.
     * Then its syndrome is calculated and used as a pointer to the table
     * where the error pattern value is stored.
     * ---------------------------------------------------------------------
     *
     * (1) Error patterns of WEIGHT 1 (SINGLE ERRORS)
     */
    decoding_table[0] = 0;
    decoding_table[1] = 1;
    temp = 1;
    for (i=2; i<= 23; i++) {
        temp *= 2;
        decoding_table[get_syndrome(temp)] = temp;
    }
    /*
     * (2) Error patterns of WEIGHT 2 (DOUBLE ERRORS)
     */
    a[1] = 1; a[2] = 2;
    temp = arr2int(a,2);
    decoding_table[get_syndrome(temp)] = temp;
    for (i=1; i<253; i++) {
        nextcomb(23,2,a);
        temp = arr2int(a,2);
        decoding_table[get_syndrome(temp)] = temp;
    }
    /*
     * (3) Error patterns of WEIGHT 3 (TRIPLE ERRORS)
     */
    a[1] = 1; a[2] = 2; a[3] = 3;
    temp = arr2int(a,3);
    decoding_table[get_syndrome(temp)] = temp;
    for (i=1; i<1771; i++) {
        nextcomb(23,3,a);
        temp = arr2int(a,3);
        decoding_table[get_syndrome(temp)] = temp;
    }
    //****************************** END GOLAY Functions ***********************************************
    /* ---------------------------------------------------------------------
     *                        decode Codeword DATA
     * ---------------------------------------------------------------------
     */
    j = 0;
    if(LRfactor == 7){
        while (j < len){
            recLR = 0x00;
            pos8 = (helperData[(j*LRfactor)] ^ sramData[(j*LRfactor)]) >> 1;
            if(majorityVoting(pos8, LRfactor)) {
                recLR = 0x80;
            }
            pos7 = ((helperData[((j*LRfactor)+1)] ^ sramData[(j*LRfactor)+1]) >> 2) ^ (((helperData[(j*LRfactor)] ^ sramData[(j*LRfactor)]) & 0x01) << 6);
            if(majorityVoting(pos7, LRfactor)){
                recLR = (recLR ^ 0x40);
            }
            pos6 = ((helperData[((j*LRfactor)+2)] ^ sramData[(j*LRfactor)+2]) >> 3) ^ (((helperData[((j*LRfactor)+1)] ^ sramData[(j*LRfactor)+1]) & 0x03) << 5);
            if(majorityVoting(pos6, LRfactor)){
                recLR = (recLR ^ 0x20);
            }
            pos5 = ((helperData[((j*LRfactor)+3)] ^ sramData[(j*LRfactor)+3]) >> 4) ^ (((helperData[((j*LRfactor)+2)] ^ sramData[(j*LRfactor)+2]) & 0x07) << 4);
            if(majorityVoting(pos5, LRfactor)){
                recLR = (recLR ^ 0x10);
            }
            pos4 = ((helperData[((j*LRfactor)+4)] ^ sramData[(j*LRfactor)+4]) >> 5) ^ (((helperData[((j*LRfactor)+3)] ^ sramData[(j*LRfactor)+3]) & 0x0F) << 3);
            if(majorityVoting(pos4, LRfactor)){
                recLR = (recLR ^ 0x08);
            }
            pos3 = ((helperData[((j*LRfactor)+5)] ^ sramData[(j*LRfactor)+5]) >> 6) ^ (((helperData[((j*LRfactor)+4)] ^ sramData[(j*LRfactor)+4]) & 0x1F) << 2);
            if(majorityVoting(pos3, LRfactor)){
                recLR = (recLR ^ 0x04);
            }
            pos2 = ((helperData[((j*LRfactor)+6)] ^ sramData[(j*LRfactor)+6]) >> 7) ^ (((helperData[((j*LRfactor)+5)] ^ sramData[(j*LRfactor)+5]) & 0x3F) << 1);
            if(majorityVoting(pos2, LRfactor)){
                recLR = (recLR ^ 0x02);
            }
            pos1 = ((helperData[((j*LRfactor)+6)] ^ sramData[(j*LRfactor)+6]) & 0x7F);
            if(majorityVoting(pos1, LRfactor)) {
                recLR = (recLR ^ 0x01);
            }
            // set correct recoverd bits
            recoverdFromLR[j] = recLR;
            j++;
        }
    }
    if(LRfactor == 15){
        while (j < len){
            recLR = 0x00;
            pos8 = ((helperData[(j*LRfactor)] ^ sramData[(j*LRfactor)]) << 7) ^ ((helperData[(j*LRfactor)+1] ^ sramData[(j*LRfactor)+1]) >> 1);
            if(majorityVoting(pos8, LRfactor)) {
                recLR = 0x80;
            }
            pos7 = ((((helperData[(j*LRfactor)+1] ^ sramData[(j*LRfactor)+1]) & 0x01) << 14) ^ ((helperData[((j*LRfactor)+2)] ^ sramData[(j*LRfactor)+2]) << 6) ^ ((helperData[(j*LRfactor)+3] ^ sramData[(j*LRfactor)+3]) >> 2));
            if(majorityVoting(pos7, LRfactor)){
                recLR = (recLR ^ 0x40);
            }
            pos6 = ((((helperData[(j*LRfactor)+3] ^ sramData[(j*LRfactor)+3]) & 0x03) << 13) ^ ((helperData[((j*LRfactor)+4)] ^ sramData[(j*LRfactor)+4]) << 5) ^ ((helperData[(j*LRfactor)+5] ^ sramData[(j*LRfactor)+5]) >> 3));
            if(majorityVoting(pos6, LRfactor)){
                recLR = (recLR ^ 0x20);
            }
            pos5 = ((((helperData[(j*LRfactor)+5] ^ sramData[(j*LRfactor)+5]) & 0x07) << 12) ^ ((helperData[((j*LRfactor)+6)] ^ sramData[(j*LRfactor)+6]) << 4) ^ ((helperData[(j*LRfactor)+7] ^ sramData[(j*LRfactor)+7]) >> 4));
            if(majorityVoting(pos5, LRfactor)){
                recLR = (recLR ^ 0x10);
            }
            pos4 = ((((helperData[(j*LRfactor)+7] ^ sramData[(j*LRfactor)+7]) & 0x0F) << 11) ^ ((helperData[((j*LRfactor)+8)] ^ sramData[(j*LRfactor)+8]) << 3) ^ ((helperData[(j*LRfactor)+9] ^ sramData[(j*LRfactor)+9]) >> 5));
            if(majorityVoting(pos4, LRfactor)){
                recLR = (recLR ^ 0x08);
            }
            pos3 = ((((helperData[(j*LRfactor)+9] ^ sramData[(j*LRfactor)+9]) & 0x1F) << 10) ^ ((helperData[((j*LRfactor)+10)] ^ sramData[(j*LRfactor)+10]) << 2) ^ ((helperData[(j*LRfactor)+11] ^ sramData[(j*LRfactor)+11]) >> 6));
            if(majorityVoting(pos3, LRfactor)){
                recLR = (recLR ^ 0x04);
            }
            pos2 = ((((helperData[(j*LRfactor)+11] ^ sramData[(j*LRfactor)+11]) & 0x3F) << 9) ^ ((helperData[((j*LRfactor)+12)] ^ sramData[(j*LRfactor)+12]) << 1) ^ ((helperData[(j*LRfactor)+13] ^ sramData[(j*LRfactor)+13]) >> 7));
            if(majorityVoting(pos2, LRfactor)){
                recLR = (recLR ^ 0x02);
            }
            pos1 = ((((helperData[(j*LRfactor)+13] ^ sramData[(j*LRfactor)+13]) & 0x7F) << 8) ^ (helperData[(j*LRfactor)+14] ^ sramData[(j*LRfactor)+14])) ;
            if(majorityVoting(pos1, LRfactor)) {
                recLR = (recLR ^ 0x01);
            }
            // set correct recoverd bits
            recoverdFromLR[j] = recLR;
            j++;
        }
    }	
    /* ---------------------------------------------------------------------
     *                        decode original DATA
     * ---------------------------------------------------------------------
     */
    k = 0;
    while(k < len){
        recd = 0;
        recd2 = 0;
        recd |= recoverdFromLR[k] << 16;
        recd |= (recoverdFromLR[k+1] << 8);
        recd |= (recoverdFromLR[k+2]);
        recd ^= decoding_table[get_syndrome(recd)];
        recd2 |= recoverdFromLR[k+3] << 16;
        recd2 |= (recoverdFromLR[k+4] << 8);
        recd2 |= (recoverdFromLR[k+5]);
        recd2 ^= decoding_table[get_syndrome(recd2)];
        // Save reconstructed secret 
        recoverdSecret[(k/2)] = (recd >> 15);
        if(((int)((((float)originalSecretSize/3.0f) - (float)((int)originalSecretSize/3))*10) == 3) && ((k+6) > (originalSecretSize*2))){break;}
        recoverdSecret[(k/2)+1] = ((((recd >> 11) & 0x000F) << 4) ^ ((recd2 >> 19) & 0x000F));
        if(((int)((((float)originalSecretSize/3.0f) - (float)((int)originalSecretSize/3))*10) == 6) && ((k+6) > (originalSecretSize*2))){break;}
        recoverdSecret[(k/2)+2] = ((recd2 >> 11) & 0x00FF);
        k = k + 6;
    }
    int ret = 1;
    if (typ == "pr")
        ret = Savefile(recoverdSecret);
    if (ret != 0)
        printf("save incomplete!!");

    free(helperData);
    //check if secrets match
    int count = 0;
    if (typ == "pu"){
        for (i=0; i<sizeof(ts2);i++){
            if (*(recoverdSecret+i) == (ts2[i])){
                count = count + 1;
            }
        }
        if (count == sizeof(ts2)){
            printf("\nYes, they match.\n\n");
        } else{
            printf("\nNo, they don't match!!!!\n\n");
        }
    }
    if (typ == "pr"){
        for (i=0; i<sizeof(ts);i++){
            if (*(recoverdSecret+i) == (ts[i])){
                count = count + 1;
            }
        }
        if (count == sizeof(ts)){
            printf("\nYes, they match.\n\n");

        } else{
            printf("\nNo, they don't match!!!!\n\n");

        }    
    }


    free(recoverdSecret);
    free(recoverdFromLR);

    return 0;
}

//###############################################################
// Array and Start Pointer for the saved SRAM DATA and HelperData
unsigned char *sram; //32720		
unsigned char *keydata;
//
//*****************************************************************************
// Length of the Secret (public- / private- Input) Key 
//
int len;
int originalSize;		
//
//*****************************************************************************
//	Set CPU clock for sync FLASH operations
//													
unsigned long ulOperatingFrequency = 20;// in MHz
//
//*****************************************************************************
int main(void)
{
    // Copy SRAM DATA from file to memory
    FILE *fd, *fd1;
    unsigned int error = 0;
    struct Item item;
    item.offset_begin = 1024;
    item.offset_end = 16;
    item.input_length = 0;
    strcpy(item.output_Key_name, "pkr");
    strcpy(item.input_PUF_name, "PUF");
    strcpy(item.input_HD_name, "/home/prankur/golay/generate/helperdata7pr.bin");
    item.HW_ENTP_mode = 0;
    item.LR = 7;
    char stri[100];
    char al[100];

    //printf("Give the filename: ");
    //scanf("%s", stri);

    //if (stri[0] == 'S'){
    //    al[0]='C';
    //    strcat(al,":\\Users\\Nikolas\\Dropbox\\Stellaris16\\");
    //    strcat(al,stri);}

    //if (al[0] == 'C')
    //    ptr = fopen(al,"rb");
    //else 
    //    ptr = fopen(stri,"rb");

    //if (!ptr)
    //{
    //    printf("Unable to open file!\n");
    //    return 1;
    //}

    //fseek(ptr, 1024 , SEEK_SET ); //48

    error = SetInputLen(&item, 0);
    //read from PUF and store in sramData
    if ((fd = fopen(item.input_PUF_name, "rb")) == NULL) {
        printf("unable to open PUF file!\n");
        return -1;
    }
    fseek(fd, item.offset_begin, SEEK_SET);
    sram = (unsigned char *) malloc(sizeof(char) * item.input_length);
    if (fread(&sram[0], sizeof(char), item.input_length, fd) != item.input_length) {
        printf("unable to read PUF file!\n");
        goto error2;
    }

    item.offset_begin = item.offset_end = 0;
    error = SetInputLen(&item, 2);
    //read from helper file and store in keydata
    if (( fd1 = fopen(item.input_HD_name, "rb")) == NULL) {
        printf("unable to open helper file\n");
        goto error2;
    }
    fseek(fd1, item.offset_begin, SEEK_SET);
    keydata = (unsigned char *) malloc(sizeof(char) * item.input_length);
    if (fread(&keydata[0], 1, item.input_length, fd1) != item.input_length) {
        printf("unable to read helper file\n");
        goto error1;
    }

    //*****************************************************************************

    // Reconstruct privateKey (length 428 byte)
    // set Input Size and calculate golay length
    originalSize = 812;
    len = ((int)(((float)(originalSize*2)/3.0f)+0.999f)*3);
    recoverOriginalData(sram, 7, originalSize, len,"pr");
    //recoverOriginalData(sram, 15, originalSize, len,"pr");

    // Reconstruct publicKey (length 128 byte)
    // set Input Size and calculate golay length
    originalSize = 128;
    len = ((int)(((float)(originalSize*2)/3.0f)+0.999f)*3);
    //recoverOriginalData(sram, 7, originalSize, len,"pu");
    //recoverOriginalData(sram, 15, originalSize, len,"pu");

error1:
    fclose(fd1);
    free(keydata);
error2:
    free(sram);
    fclose(fd);

    return 0;
}
