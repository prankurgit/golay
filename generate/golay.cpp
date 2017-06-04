//*****************************************************************************
//
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
            if((fd = fopen(item->input_Key_name, "rb")) == NULL) return 12;
            break;
        case 2:
            if((fd = fopen(item->input_file_name, "rb")) == NULL) return 12;
            break;
        default:
            printf("incorrect option!\n");
            return -1;
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

void DefineFilename_BCH(struct Item *item, int option)
/*
 * Define the filename
 *
 * Inputs:
 * item = pointer to the struct to store the necessary informations
 * option = definition if the input Key / PUF or output HD file will be specified
 *
 * Options:
 * 1 = input Key filename
 * 2 = input PUF filename
 * 3 = output HD filename
 * 4 = output Key filename 
 * 5 = input HD filename
 */
{
    char *h;
    unsigned int ch;
    char name[52];
    unsigned int error = 0;


    while(true){
        cout << "*******************************************************************************" << endl;
        cout << "*                                                                             *" << endl;
        if(option == 1)
        cout << "*          Set the 'File-Name' of the file in which the Key is stored         *" << endl;
        if(option == 2){
        cout << "*                   Set the 'File-Name' of the PUF-Response                   *" << endl;}
        if(option == 3) {
        cout << "*   Set the 'File-Name' of the file in which the HelperData will be stored    *" << endl;}
        if(option == 4) {
        cout << "*   Set the 'File-Name' of the file in which the decoded Key will be stored   *" << endl;}
        if(option == 5) {
        cout << "*   Set the 'File-Name' of the file in which the HelperData is stored         *" << endl;}
        cout << "*                                                                             *" << endl;
        cout << "*******************************************************************************" << endl;
        cout << "*                                                                             *" << endl;
        if(option == 1 || option == 2 || option == 5) {
        cout << "* If the file is in the current working directory:                            *" << endl;}
        if(option == 3 || option == 4){
        cout << "* If the file should be stored in the current working directory:              *" << endl;}
        cout << "*                                                                             *" << endl;
        if(option == 1) {
        cout << "*  -> Use only the filename               = key_file.txt                      *" << endl;}
        if(option == 2) {
        cout << "*  -> Use only the filename               = Stellaris_PUF1                    *" << endl;}
        if(option == 3) {
        cout << "*  -> Use only the filename               = HelperData_1                      *" << endl;}
        if(option == 4) {
        cout << "*  -> Use only the filename               = key_file.txt                      *" << endl;}
        if(option == 5) {
        cout << "*  -> Use only the filename               = HelperData_1                      *" << endl;}
        cout << "*                                                                             *" << endl;
        if(option == 1 || option == 2 || option == 5) {
        cout << "* If the file is in an sub-folder of the current working directory:           *" << endl;}
        if(option == 3 || option == 4) {
        cout << "* If the file should be stored in an sub-folder of the current directory:     *" << endl;}
        cout << "*                                                                             *" << endl;
        if(option == 1) {
        cout << "*  -> Use the relative path and filename  = device1/key_file.txt              *" << endl;}
        if(option == 2) {
        cout << "*  -> Use the relative path and filename  = device1/Stellaris_PUF1            *" << endl;}
        if(option == 3) {
        cout << "*  -> Use the relative path and filename  = device1/HelperData_1              *" << endl;}
        if(option == 4) {
        cout << "*  -> Use the relative path and filename  = device1/key_file.txt              *" << endl;}
        if(option == 5) {
        cout << "*  -> Use the relative path and filename  = device1/HelperData_1              *" << endl;}
        cout << "*                                                                             *" << endl;
        if(option == 1 || option == 2 || option == 5) {
        cout << "* If the file is not in an sub-folder of the current working directory:       *" << endl;}
        if(option == 3 || option == 4) {
        cout << "* If the file should be stored somewhere else:                                *" << endl;}
        cout << "*                                                                             *" << endl;
        if(option == 1) {
        cout << "*  -> Use the full path and filename      = C:/User/Data/device1/key_file.txt *" << endl;}
        if(option == 2) {
        cout << "*  -> Use the full path and filename      = C:/Data/device1/Stellaris_PUF1    *" << endl;}
        if(option == 3) {
        cout << "*  -> Use the full path and filename      = C:/Data/device1/HelperData_1      *" << endl;}
        if(option == 4) {
        cout << "*  -> Use the full path and filename      = C:/User/Data/device1/key_file.txt *" << endl;}
        if(option == 5) {
        cout << "*  -> Use the full path and filename      = C:/Data/device1/HelperData_1      *" << endl;}
        cout << "*                                                                             *" << endl;
        cout << "*******************************************************************************" << endl;

        if(error) {
            ErrorMessages(error, 0, 0);
        }
        error = 0;

        cout << endl << "Type in the (Path and) Filename : ";
        if (fgets(name, sizeof(name), stdin)) {
            /* fgets succeeds, scan for newline character */
            h = strchr(name, '\n');
                if (h) {
                    if(name[0] == '\n') error = 6;
                    else {
                            *h = '\0';
							// Set input/ouput filename
                            if (option == 1) strcpy (item->input_Key_name ,name);
                            else if (option == 2) strcpy (item->input_PUF_name,name);
                            else if (option == 3) strcpy (item->output_HD_name,name);
                            else if (option == 4) strcpy (item->output_Key_name,name);
                            else if (option == 5) strcpy (item->input_HD_name,name);
                    }
                }
                else {
                    /* newline not found, flush stdin to end of line */
                        while (((ch = getchar()) != '\n')
                            && !feof(stdin)
                            && !ferror(stdin)
                        );
                        error = 5;
                }
          }
          else {
            /* fgets failed, handle error */
            error = 3;
          }
          if(!error) break;
    }
}

void DefineSettings(struct Item *item, int option)
/*
 * Function to get the 'offset' and 'Linear Repetition factor' as user input
 *
 * Inputs:
 * item  = pointer to the struct to store the necessary informations
 * option = definition if the offset or LR factor will be specified
 *
 * Options:
 * 1 = Offset, the amount of bytes (in decimal) that will be skipped from the beginning
 * 2 = Linear Repetition factor should be 7 or 15
 */
{
    char oSet[12];
    char lr[4];
    char *h;
    unsigned int ch, i = 0;
    unsigned int error = 0;

    if(option == 1){
        while(true){
            cout << "*******************************************************************************" << endl;
            cout << "*                                                                             *" << endl;
            cout << "*                  Define the 'offset' for the PUF-Response:                  *" << endl;
            cout << "*                                                                             *" << endl;
            cout << "*                   0  = use the PUF-Response from beginning on               *" << endl;
            cout << "*                   1  = the first byte will be skipped                       *" << endl;
            cout << "*                  'x' = the first 'x' bytes will be skipped                  *" << endl;
            cout << "*                                                                             *" << endl;
            cout << "*******************************************************************************" << endl;

            if(error) ErrorMessages(error, i, 0);
            error = 0;

            cout << endl << "Type in the offset in bytes (as decimal number): ";
            if (fgets(oSet, sizeof(oSet), stdin)) {
                /* fgets succeeds, scan for newline character */
                h = strchr(oSet, '\n');
                if (h) {
                    *h = '\0';
                    //check input if only digits are used
                    for(i = 0; i < sizeof(oSet)-1; i++){
                        if(oSet[i] != '\0' && !isdigit(oSet[i])){
                            error = 1;
                            break;
                        }
                            if(oSet[i] == '\0') i = sizeof(oSet);
                    }
                    if(error == 0){

                        // Set the offSet
                        item->offSet = atol(oSet);
                        break;
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
                error = 3;
            }
            if(!error) break;
        }
    }
    else if(option == 2){
        while(true){
            cout << "*******************************************************************************" << endl;
            cout << "*                                                                             *" << endl;
            cout << "*                  Define the factor for the Linear Repetition:               *" << endl;
            cout << "*                                                                             *" << endl;
            cout << "*                   7   = each bit will be repeated seven times               *" << endl;
            cout << "*                   15  = each bit will be repeated fifteen times             *" << endl;
            cout << "*                                                                             *" << endl;
            cout << "*******************************************************************************" << endl;

            if(error) ErrorMessages(error, i, 0);
            error = 0;

            // Get "LR factor" as user input
            cout << endl << "Enter the Linear Repetition factor (7 or 15): ";
            if (fgets(lr, sizeof(lr), stdin)) {
                /* fgets succeeds, scan for newline character */
                h = strchr(lr, '\n');
                    if (h) {
                        if(lr[0] == '\n') error = 6;
                        else {
                            *h = '\0';
                            //check input if only digits are used
                            for(i = 0; i < (signed)sizeof(lr)-1; i++){
                                if(lr[i] != '\0' && !isdigit(lr[i])){
                                    error = 1;
                                    break;
                                }
                                if(lr[i] == '\0') i = sizeof(lr);
                            }
                            if(!error){
                                if(atoi(lr) == 7 || atoi(lr) == 15){
                                    item->LR = atoi(lr);
                                    break;
                                }
                                else error = 4;
                            }
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
                error = 3;
            }
        }
    }
}

int n, r;
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
//*****************************************************************************

long get_syndrome(long pattern)
/*
 * Compute the syndrome corresponding to the given pattern, i.e., the
 * remainder after dividing the pattern (when considering it as the vector
 *
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
// ########################## START ENCODED CODEWORD ARRAY ##################
int generateHelperData(volatile unsigned char *inputSecret, int inputSize, volatile unsigned char *sram, int LRfactor, const char *typ )
{
    //
    // Variables of this function	
    int left = 8;
    int v = 0;
    int fa = 0;
    int i = 0;
    int j = 0;

    long data, codeword, temp;
    int len = (((int)(((float)(inputSize*2)/3.0f)+0.999f))*3);
    unsigned char *codeWordArrayBeforLR = (unsigned char*)malloc(sizeof(unsigned char) * len); 
    unsigned char *encodeLR = (unsigned char*)malloc(sizeof(unsigned char) * (LRfactor*len)); 
    //*****************************************************************************
    /*
     * ---------------------------------------------------------------------
     *                  Generate ENCODING TABLE - GOLAY
     *
     * An entry to the table is an information vector, a 32-bit integer,
     * whose 12 least significant positions are the information bits. The
     * resulting value is a codeword in the (23,12,7) Golay code: A 32-bit
     * integer whose 23 least significant bits are coded bits: Of these, the
     * 12 most significant bits are information bits and the 11 least
     * significant bits are redundant bits (systematic encoding).
     * ---------------------------------------------------------------------
     */
    for (pattern = 0; pattern < 4096; pattern++) {
        temp = pattern << 11;          /* multiply information by X^{11} */
        encoding_table[pattern] = temp + get_syndrome(temp);/* add redundancy */
    }	
    //*****************************************************************************
    // Generate 12 bit for Golay codeword generation

    while (j < inputSize){
        v = 0;
        switch (left)
        {
            case 8:
                {
                    v = inputSecret[j++];
                    if(j == inputSize){			// correction
                        v = (v << 4) | (0x00);	// correction
                    }							// correction
                    else{						// correction
                        v = (v << 4) | (inputSecret[j] >> 4); 
                    }							// correction
                    left = 4;
                    break;
                }
            case 4:
                {
                    v = inputSecret[j++] & 0x0F; 							// lower nibble
                    v = (v << 8) | inputSecret[j++];  					
                    left = 8;
                    break;
                }
        }
        data = (long) v;
        // Codeword generation and into ARRAY
        codeword = encoding_table[data];
        codeWordArrayBeforLR[(fa)] = (codeword >> 16) & 0xFF;       
        codeWordArrayBeforLR[(fa+1)] = (codeword >> 8) & 0xFF;       
        codeWordArrayBeforLR[(fa+2)] = codeword & 0xFF;               
        // Step for next 3
        fa = (fa+3);
    }		
    // Perform Linear Repetition
    if(LRfactor == 7){
        while( i < len){
            if((codeWordArrayBeforLR[i] & 0x80) == 0x00){                       // MSB (bit at position 8 == 0)
                encodeLR[(i*LRfactor)] = 0x00;
            }
            else {                                                              // MSB (bit at position 8 == 1)
                encodeLR[(i*LRfactor)] = 0xFE;
            }
            if((codeWordArrayBeforLR[i] & 0x40) == 0x00){                       // (bit at position 7 == 0)
                encodeLR[(i*LRfactor)+1] = 0x00;
            } else {                                                            // (bit at position 7 == 1)
                encodeLR[(i*LRfactor)] = encodeLR[(i*LRfactor)] ^ 0x01;
                encodeLR[(i*LRfactor)+1] = 0xFC;
            }
            if((codeWordArrayBeforLR[i] & 0x20) == 0x00){                       // (bit at position 6 == 0)
                encodeLR[(i*LRfactor)+2] = 0x00;
            } else {                                                            // (bit at position 6 == 1)
                encodeLR[(i*LRfactor)+1] = encodeLR[(i*LRfactor)+1] ^ 0x03;
                encodeLR[(i*LRfactor)+2] = 0xF8;
            }
            if((codeWordArrayBeforLR[i] & 0x10) == 0x00){                       // (bit at position 5 == 0)
                encodeLR[(i*LRfactor)+3] = 0x00;
            } else {                                                            // (bit at position 5 == 1)
                encodeLR[(i*LRfactor)+2] = encodeLR[(i*LRfactor)+2] ^ 0x07;
                encodeLR[(i*LRfactor)+3] = 0xF0;
            }
            if((codeWordArrayBeforLR[i] & 0x08) == 0x00){                       // (bit at position 4 == 0)
                encodeLR[(i*LRfactor)+4] = 0x00;
            } else {                                                            // (bit at position 4 == 1)
                encodeLR[(i*LRfactor)+3] = encodeLR[(i*LRfactor)+3] ^ 0x0F;
                encodeLR[(i*LRfactor)+4] = 0xE0;
            }
            if((codeWordArrayBeforLR[i] & 0x04) == 0x00){                       // (bit at position 3 == 0)
                encodeLR[(i*LRfactor)+5] = 0x00;
            } else {                                                            // (bit at position 3 == 1)
                encodeLR[(i*LRfactor)+4] = encodeLR[(i*LRfactor)+4] ^ 0x1F;		
                encodeLR[(i*LRfactor)+5] = 0xC0;																							
            }
            if((codeWordArrayBeforLR[i] & 0x02) == 0x00){                       // (bit at position 2 == 0)
                encodeLR[(i*LRfactor)+6] = 0x00;
            } else {                                                            // (bit at position 2 == 1)
                encodeLR[(i*LRfactor)+5] = encodeLR[(i*LRfactor)+5] ^ 0x3F;
                encodeLR[(i*LRfactor)+6] = 0x80;
            }
            if((codeWordArrayBeforLR[i] & 0x01) == 0x00){                       // (bit at position 1 == 0)
                encodeLR[(i*LRfactor)+6] = encodeLR[(i*LRfactor)+6] ^ 0x00;	
            } else {                                                            // (bit at position 1 == 1
                encodeLR[(i*LRfactor)+6] = encodeLR[(i*LRfactor)+6] ^ 0x7F;
            }	
            encodeLR[(i*LRfactor)] = (encodeLR[(i*LRfactor)] ^ sram[(i*LRfactor)]);
            encodeLR[(i*LRfactor)+1] = (encodeLR[(i*LRfactor)+1] ^ sram[(i*LRfactor)+1]);
            encodeLR[(i*LRfactor)+2] = (encodeLR[(i*LRfactor)+2] ^ sram[(i*LRfactor)+2]);
            encodeLR[(i*LRfactor)+3] = (encodeLR[(i*LRfactor)+3] ^ sram[(i*LRfactor)+3]);
            encodeLR[(i*LRfactor)+4] = (encodeLR[(i*LRfactor)+4] ^ sram[(i*LRfactor)+4]);
            encodeLR[(i*LRfactor)+5] = (encodeLR[(i*LRfactor)+5] ^ sram[(i*LRfactor)+5]);
            encodeLR[(i*LRfactor)+6] = (encodeLR[(i*LRfactor)+6] ^ sram[(i*LRfactor)+6]);		
            i++;
        } 
    }
    if(LRfactor == 15){
        while( i < len){
            if((codeWordArrayBeforLR[i] & 0x80) == 0x00){                                           // MSB (bit at position 8 == 0)
                encodeLR[(i*LRfactor)] = 0x00;
                encodeLR[((i*LRfactor)+1)] = 0x00;
            }
            else {                                                              										// MSB (bit at position 8 == 1)
                encodeLR[(i*LRfactor)] = 0xFF;
                encodeLR[((i*LRfactor)+1)] = 0xFE;
            }
            if((codeWordArrayBeforLR[i] & 0x40) == 0x00){                                           // (bit at position 7 == 0)
                encodeLR[((i*LRfactor)+2)] = 0x00;
                encodeLR[((i*LRfactor)+3)] = 0x00;
            } else {                                                            										// (bit at position 7 == 1)
                encodeLR[((i*LRfactor)+1)] = encodeLR[((i*LRfactor)+1)] ^ 0x01;
                encodeLR[((i*LRfactor)+2)] = 0xFF;
                encodeLR[((i*LRfactor)+3)] = 0xFC;
            }
            if((codeWordArrayBeforLR[i] & 0x20) == 0x00){                                           // (bit at position 6 == 0)
                encodeLR[((i*LRfactor)+4)] = 0x00;
                encodeLR[((i*LRfactor)+5)] = 0x00;
            } else {                                                            										// (bit at position 6 == 1)
                encodeLR[((i*LRfactor)+3)] = encodeLR[((i*LRfactor)+3)] ^ 0x03;
                encodeLR[((i*LRfactor)+4)] = 0xFF;
                encodeLR[((i*LRfactor)+5)] = 0xF8;
            }
            if((codeWordArrayBeforLR[i] & 0x10) == 0x00){                                           // (bit at position 5 == 0)
                encodeLR[((i*LRfactor)+6)] = 0x00;
                encodeLR[((i*LRfactor)+7)] = 0x00;
            } else {                                                            										// (bit at position 5 == 1)
                encodeLR[((i*LRfactor)+5)] = encodeLR[((i*LRfactor)+5)] ^ 0x07;
                encodeLR[((i*LRfactor)+6)] = 0xFF;
                encodeLR[((i*LRfactor)+7)] = 0xF0;
            }
            if((codeWordArrayBeforLR[i] & 0x08) == 0x00){                                           // (bit at position 4 == 0)
                encodeLR[((i*LRfactor)+8)] = 0x00;
                encodeLR[((i*LRfactor)+9)] = 0x00;
            } else {                                                            										// (bit at position 4 == 1)
                encodeLR[((i*LRfactor)+7)] = encodeLR[((i*LRfactor)+7)] ^ 0x0F;
                encodeLR[((i*LRfactor)+8)] = 0xFF;
                encodeLR[((i*LRfactor)+9)] = 0xE0;
            }
            if((codeWordArrayBeforLR[i] & 0x04) == 0x00){                                           // (bit at position 3 == 0)
                encodeLR[((i*LRfactor)+10)] = 0x00;
                encodeLR[((i*LRfactor)+11)] = 0x00;
            } else {                                                            										// (bit at position 3 == 1)
                encodeLR[((i*LRfactor)+9)] = encodeLR[((i*LRfactor)+9)] ^ 0x1F;
                encodeLR[((i*LRfactor)+10)] = 0xFF;
                encodeLR[((i*LRfactor)+11)] = 0xC0;
            }
            if((codeWordArrayBeforLR[i] & 0x02) == 0x00){                                           // (bit at position 2 == 0)
                encodeLR[((i*LRfactor)+12)] = 0x00;
                encodeLR[((i*LRfactor)+13)] = 0x00;
            } else {                                                           											// (bit at position 2 == 1)
                encodeLR[((i*LRfactor)+11)] = encodeLR[((i*LRfactor)+11)] ^ 0x3F;
                encodeLR[((i*LRfactor)+12)] = 0xFF;
                encodeLR[((i*LRfactor)+13)] = 0x80;
            }
            if((codeWordArrayBeforLR[i] & 0x01) == 0x00){                                           // (bit at position 1 == 0)
                encodeLR[((i*LRfactor)+13)] = encodeLR[((i*LRfactor)+13)] ^ 0x00;
                encodeLR[((i*LRfactor)+14)] = 0x00;	
            } else {                                                            										// (bit at position 1 == 1
                encodeLR[((i*LRfactor)+13)] = encodeLR[((i*LRfactor)+13)] ^ 0x7F;
                encodeLR[((i*LRfactor)+14)] = 0xFF;
            }
            encodeLR[(i*LRfactor)] = (encodeLR[(i*LRfactor)] ^ sram[(i*LRfactor)]);
            encodeLR[(i*LRfactor)+1] = (encodeLR[(i*LRfactor)+1] ^ sram[(i*LRfactor)+1]);
            encodeLR[(i*LRfactor)+2] = (encodeLR[(i*LRfactor)+2] ^ sram[(i*LRfactor)+2]);
            encodeLR[(i*LRfactor)+3] = (encodeLR[(i*LRfactor)+3] ^ sram[(i*LRfactor)+3]);
            encodeLR[(i*LRfactor)+4] = (encodeLR[(i*LRfactor)+4] ^ sram[(i*LRfactor)+4]);
            encodeLR[(i*LRfactor)+5] = (encodeLR[(i*LRfactor)+5] ^ sram[(i*LRfactor)+5]);
            encodeLR[(i*LRfactor)+6] = (encodeLR[(i*LRfactor)+6] ^ sram[(i*LRfactor)+6]);	
            encodeLR[(i*LRfactor)+7] = (encodeLR[(i*LRfactor)+7] ^ sram[(i*LRfactor)+7]);
            encodeLR[(i*LRfactor)+8] = (encodeLR[(i*LRfactor)+8] ^ sram[(i*LRfactor)+8]);
            encodeLR[(i*LRfactor)+9] = (encodeLR[(i*LRfactor)+9] ^ sram[(i*LRfactor)+9]);
            encodeLR[(i*LRfactor)+10] = (encodeLR[(i*LRfactor)+10] ^ sram[(i*LRfactor)+10]);
            encodeLR[(i*LRfactor)+11] = (encodeLR[(i*LRfactor)+11] ^ sram[(i*LRfactor)+11]);
            encodeLR[(i*LRfactor)+12] = (encodeLR[(i*LRfactor)+12] ^ sram[(i*LRfactor)+12]);						
            encodeLR[(i*LRfactor)+13] = (encodeLR[(i*LRfactor)+13] ^ sram[(i*LRfactor)+13]);
            encodeLR[(i*LRfactor)+14] = (encodeLR[(i*LRfactor)+14] ^ sram[(i*LRfactor)+14]);	
            i++;
        } 
    }

    // print to file
    FILE *ptr2;
    if ((ptr2 = fopen("helperdata.bin", "wb")) == NULL)
    {
        printf("Unable to open helper data file!\n");
        return -1;
    }

    fwrite(encodeLR,sizeof(unsigned char),(LRfactor*len), ptr2);

    fclose(ptr2);

    free(codeWordArrayBeforLR);
    free(encodeLR);		
    return 0;
}
//############################## END ENCODED CODEWORD ARRAY ########################

// Array for the saved SRAM DATA
unsigned char *sramData; //32720
unsigned char *keydata; 

//
//*****************************************************************************
int main(void)
{
    FILE *fd;
    FILE *fd1;
    unsigned int error = 0;
    struct Item item;
    item.offset_begin = 1024;//for the time being harcoded to match the length
    item.offset_end = 16;   // in the initial code
    item.input_length = 0;
    strcpy(item.input_Key_name, "pubk");
    strcpy(item.input_PUF_name, "PUF");
    item.HD_error_pos = 0;
    item.HW_ENTP_mode = 0;
    item.LR = 7;
    const char * ktype = "pr";

    error = SetInputLen(&item, 0);
    //read from PUF and store in sramData
    if ((fd = fopen(item.input_PUF_name,"rb")) == NULL) {
        printf("unable to open PUF file!\n");
        return -1;
    }
    fseek(fd, item.offset_begin, SEEK_SET);

    sramData = (unsigned char *) malloc(sizeof(char) * item.input_length);

    if (fread(&sramData[0], sizeof(char), item.input_length, fd) != item.input_length) {
        printf("unable to read PUF file\n");
        goto error2;
    }

    item.offset_begin = item.offset_end = 0;
    error = SetInputLen(&item, 1);
    //read from key file and store in keydata
    if ((fd1 = fopen(item.input_Key_name, "rb")) == NULL) {
        printf("unable to open Key file!\n");
        goto error2;
    }
    fseek(fd1, item.offset_begin, SEEK_SET);
    keydata = (unsigned char *) malloc(sizeof(char) * item.input_length);

    if (fread(&keydata[0], sizeof(char), item.input_length, fd1) != item.input_length) {
        printf("unable to read Key file\n");
        goto error1;
    }

    /* ---------------------------------------------------------------------
     *                        Generate Codeword DATA (with LR)
     * ---------------------------------------------------------------------
     */

    // functioncall to generate the codewords array and perform LR and save HelperData on FLASH for the privateKey (ts)
    generateHelperData(keydata, item.input_length, sramData, item.LR, ktype);

error1:
    fclose(fd1);
    free(keydata);
error2:
    free(sramData);
    fclose(fd);
    return 0;
}
