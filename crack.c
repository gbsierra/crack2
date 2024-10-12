#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "md5.h"

const int PASS_LEN = 20;        // Maximum any password will be
const int HASH_LEN = 33;        // Length of MD5 hash strings


//tryWord()
char * tryWord(char * plaintext, char * hashFilename) {
    
    // Hash the plaintext
    char *hash = md5(plaintext, strlen(plaintext));
    //check for error
    if(!hash){
        return NULL;
    }

    // Open the hash file
    FILE *hashFile = fopen(hashFilename, "r");
    //check for error
    if(!hashFile){
        free(hash);
        return NULL;
    }

    char str[HASH_LEN];
    // Loop through the hash file, one line at a time.
    while(fgets(str, HASH_LEN, hashFile) != NULL){      

        //trim newline
        char *nl = strchr(str, '\n');
        if(nl) *nl = '\0';

        // Attempt to match the hash from the file to the hash of the plaintext.
        // If there is a match, you'll return the hash.

        if(!strcmp(str, hash)){
            fclose(hashFile);
            return hash;
        }
    }
    // If not, return NULL.
        // Before returning, do any needed cleanup:
        //   Close files?
        //   Free memory?
    fclose(hashFile);
    free(hash);
    return NULL;
}

    

//Main Function
int main(int argc, char *argv[]) {
    
    //check if correct amount of arguments entered
    if (argc < 3) 
    {
        fprintf(stderr, "Usage: %s hash_file dict_file\n", argv[0]);
        exit(1);
    }

    //open dictionary file in 'read'
    FILE *dictFile = fopen(argv[2], "r");

    //if cant read file
    if(!dictFile){
        printf("Can't read file!\n");
        exit(1);
    }


    // For each dictionary word, pass it to tryWord, which
    // will attempt to match it against the hashes in the hash_file.
    char dictWord[PASS_LEN];
    int num=0;
    while(!feof(dictFile)){  
        
        fgets(dictWord, PASS_LEN, dictFile);

        //trim newline
        char *nl = strchr(dictWord, '\n');
        if(nl) *nl = '\0';

        //test for a match
        char *found = tryWord(dictWord, argv[1]);
    
        // If we got a match, display the hash and the word. For example:
        //   5d41402abc4b2a76b9719d911017c592 hello
        if(found){

            printf("%s %s\n", found, dictWord);
            num++;
            free(found);
        }

    }
    
    
    // Close the dictionary file.
    fclose(dictFile);
    // Display the number of hashes that were cracked.
    printf("Number of hashes cracked: %d\n", num);
    // Free up any malloc'd memory?
    
    

}

