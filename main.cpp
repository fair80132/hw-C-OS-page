#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <stdlib.h>
#include <cstdlib>

using namespace std;

const int ASC_ZERO = 48 ;

string getInput( string filename, int & pageFrame ) ;
void InitialArray( char charray[], int pageFrame ) ;

void FIFO( string pageReference, int pageFrame, string filename ) ;
void LRU( string pageReference, int pageFrame, string filename ) ;
void AdditionalReferenceBits( string pageReference, int pageFrame, string filename ) ;
void SecondChance( string pageReference, int pageFrame, string filename ) ;
void LeastFrequentlyUsed( string pageReference, int pageFrame, string filename ) ;
void MostFrequentlyUsed( string pageReference, int pageFrame, string filename ) ;

int main() {

  string filename ;

  cout << "Enter filename(ex:input01.txt) :" ;
  cin >> filename ;

  int pageFrame = 0 ;
  string pageReference = getInput( filename, pageFrame ) ;

  /*
  cout << "PageFrame:" << gPageFrame
       << "Page Reference:" << getInput( filename ) << endl ;
  */

  FIFO( pageReference, pageFrame, filename ) ;
  LRU( pageReference, pageFrame, filename ) ;
  AdditionalReferenceBits( pageReference, pageFrame, filename ) ;
  SecondChance( pageReference, pageFrame, filename ) ;
  LeastFrequentlyUsed( pageReference, pageFrame, filename ) ;
  MostFrequentlyUsed( pageReference, pageFrame, filename ) ;


} // main

void FIFO( string pageReference, int pageFrame, string filename ) {

  int pageFault = 0 ;
  int pageReplaces = 0 ;

  filename = "output_" + filename ;
  ofstream outfile( filename.c_str(), ios::app ) ;
  outfile << "--------------FIFO-----------------------" << endl ;

  char page[pageFrame] ;
  InitialArray( page, pageFrame ) ;
  int timeStamp[pageFrame] ;
  for ( int i = 0 ; i < pageFrame ; i++ ) // initial time stamp array ;
    timeStamp[i] = 0 ;

  int timer = 0 ;

  for ( int current = 0 ; current < pageReference.size() ; current++, timer++ ) {

    int smallestTimeIndex = -1 ;
    int smallestTimeRecord = 9999 ;
    bool bool_pageFault = false ;
    for ( int j = 0 ; j < pageFrame ; j++ ) {
      // find the time stamp smallest to change ;

      if ( timeStamp[j] < smallestTimeRecord ) {
        smallestTimeRecord = timeStamp[j] ;
        smallestTimeIndex = j ;
      } // if

    } // for


    bool alreadyInPage = false ;
    for ( int j = 0 ; j < pageFrame ; j++ ) {
      // if already in page ;

      if ( page[j] == pageReference.at(current) ) {
        alreadyInPage = true ;
        break ;
      } // if

    } // for

    if ( !alreadyInPage ) {
      pageFault++ ;
      bool_pageFault = true ;
      if ( pageFault > pageFrame )
        pageReplaces++ ;
    } // if

    for ( int j = pageFrame-1 ; !alreadyInPage && j > 0 ; j-- ) {

      if ( j-1 >= 0 ) {
        page[j] = page[j-1] ;
        page[j-1] = '\0' ;
        timeStamp[j] = timeStamp[j-1] ;
        timeStamp[j-1] = 0 ;
      } // if

    } // for

    if ( !alreadyInPage ) {
      page[0] = pageReference.at(current) ;
      timeStamp[0] = timer ;
    } // if


    outfile << pageReference.at(current) << "\t" ;
    for ( int j = 0 ; j < pageFrame ; j++ ) {
      if ( page[j] != '\0' )
        outfile << page[j] ;
    } // for
    outfile << "\t" ;
    if ( bool_pageFault )
      outfile << "F" ;
    outfile << endl ;

  } // for

  outfile << "Page Fault = " << pageFault << "  Page Replaces = " << pageReplaces <<
             " Page Frames = " << pageFrame << endl ;

} // FIFO

void LRU( string pageReference, int pageFrame, string filename ) {

  int pageFault = 0 ;
  int pageReplaces = 0 ;

  filename = "output_" + filename ;
  ofstream outfile( filename.c_str(), ios::app ) ;
  outfile << "--------------LRU--------------------------" << endl ;

  char page[pageFrame] ;
  InitialArray( page, pageFrame ) ;
  int timeStamp[pageFrame] ;
  for ( int i = 0 ; i < pageFrame ; i++ ) // initial time stamp array ;
    timeStamp[i] = 0 ;

  int timer = 0 ;

  for ( int current = 0 ; current < pageReference.size() ; current++, timer++ ) {

    int smallestTimeIndex = -1 ;
    int smallestTimeRecord = 9999 ;
    bool bool_pageFault = false ;
    for ( int j = 0 ; j < pageFrame ; j++ ) {
      // find the time stamp smallest to change ;

      if ( timeStamp[j] < smallestTimeRecord ) {
        smallestTimeRecord = timeStamp[j] ;
        smallestTimeIndex = j ;
      } // if

    } // for


    bool alreadyInPage = false ;

    int alreadyIndex = -1 ;
    for ( int j = 0 ; j < pageFrame ; j++ ) {
      // if already in page ;

      if ( page[j] == pageReference.at(current) ) {
        alreadyInPage = true ;
        timeStamp[j] = timer ;
        alreadyIndex = j ;
        break ;
      } // if

    } // for


    if ( !alreadyInPage ) {
      pageFault++ ;
      bool_pageFault = true ;
      if ( pageFault > pageFrame )
        pageReplaces++ ;
    } // if

    int jsize = pageFrame-1 ;
    /*
    if ( pageFault > pageFrame ) {
      jsize = smallestTimeIndex ;
    } // if
    else if ( pageFault <= pageFrame )
      jsize = pageFrame-1 ;
    */
    for ( int j = jsize ; j > 0 ; j-- ) {

      if ( j-1 >= 0 && j-1 != alreadyIndex ) {
        page[j] = page[j-1] ;
        page[j-1] = '\0' ;
        timeStamp[j] = timeStamp[j-1] ;
        timeStamp[j-1] = 0 ;
      } // if

    } // for

    // if ( !alreadyInPage ) {
      page[0] = pageReference.at(current) ;
      timeStamp[0] = timer ;
    // } // if
    /*
    else {
      int tempPage = page[0] ;
      int tempTime = timeStamp[0] ;
      page[0] = page[smallestTimeIndex] ;
      timeStamp[0] = timer ;
      for ( int j = alreadyIndex ; j > 1 ; j-- ) {

        if ( j-1 >= 0 ) {
          page[j] = page[j-1] ;
          page[j-1] = '\0' ;
          timeStamp[j] = timeStamp[j-1] ;
          timeStamp[j-1] = 0 ;
        } // if

      } // for
      page[1] = tempPage ;
      timeStamp[1] = tempTime ;

    } // else
    */
    outfile << pageReference.at(current) << "\t" ;
    for ( int j = 0 ; j < pageFrame ; j++ ) {
      if ( page[j] != '\0' )
        outfile << page[j] ;
    } // for
    outfile << "\t" ;
    if ( bool_pageFault )
      outfile << "F" ;
    outfile << endl ;

  } // for

  outfile << "Page Fault = " << pageFault << "  Page Replaces = " << pageReplaces <<
             " Page Frames = " << pageFrame << endl ;

} // LRU

void AdditionalReferenceBits( string pageReference, int pageFrame, string filename ) {

  int pageFault = 0 ;
  int pageReplaces = 0 ;

  filename = "output_" + filename ;
  ofstream outfile( filename.c_str(), ios::app ) ;
  outfile << "--------------Additional Reference Bits-------------------" << endl ;

  char page[pageFrame] ;
  InitialArray( page, pageFrame ) ;

  int ReferenceBit[pageFrame] ;
  char ShiftRegister[pageFrame][8] ;
  for ( int i = 0 ; i < pageFrame ; i++ ) // initial time stamp array ;
    ReferenceBit[i] = 0 ;
  for ( int i = 0 ; i < pageFrame ; i++ ) {

    for ( int j = 0 ; j < 8 ; j++ )
      ShiftRegister[i][j] = '0' ;

  } // for

  int timer = 1 ;

  for ( int current = 0 ; current < pageReference.size() ; current++ ) {

    int smallestTimeIndex = -1 ;
    int smallestTimeRecord = 9999 ;
    bool bool_pageFault = false ;
    for ( int j = 0 ; j < pageFrame ; j++ ) {
      // find the time stamp smallest to change ;
      int thisBits = 0 ;
      for ( int i = 0 ; i < 8 ; i++ )
        thisBits = thisBits*2 + ShiftRegister[j][i] - ASC_ZERO ;

      if ( thisBits < smallestTimeRecord ) {
        smallestTimeRecord = thisBits ;
        smallestTimeIndex = j ;
      } // if

    } // for

    bool alreadyInPage = false ;
    int already_index = 0 ;
    for ( int j = 0 ; j < pageFrame ; j++ ) {
      // if already in page ;
      if ( page[j] == pageReference.at(current) ) {
        alreadyInPage = true ;
        ReferenceBit[j] = 1 ;
        already_index = j ;
        break ;
      } // if

    } // for

    if ( !alreadyInPage ) {
      pageFault++ ;
      bool_pageFault = true ;
      if ( pageFault > pageFrame )
        pageReplaces++ ;
    } // if

    int jsize = 0 ;
    if ( pageFault > pageFrame ) {
      jsize = smallestTimeIndex ;
    } // if
    else if ( pageFault <= pageFrame )
      jsize = pageFrame-1 ;

    for ( int j = jsize ; !alreadyInPage && j > 0 ; j-- ) {
      // shift ;
      // when the current page reference has already in the page(any page),
      // Should NOT be Shift in the round ;
      if ( j-1 >= 0 ) {
        page[j] = page[j-1] ;
        page[j-1] = '\0' ;
        for ( int i = 0 ; i < 8 ; i++ )
          ShiftRegister[j][i] = ShiftRegister[j-1][i] ;
        for ( int i = 0 ; i < 8 ; i++ )
          ShiftRegister[j-1][i] = 0 ;
      } // if

    } // for


    if ( !alreadyInPage ) {
      page[0] = pageReference.at(current) ;
      ReferenceBit[0] = 1 ;
      for ( int i = 0 ; i < 8 ; i++ )
        ShiftRegister[0][i] = '0' ;

    } // if
    else
      ReferenceBit[already_index] = 1 ;

    for ( int i = 0 ; i < pageFrame ; i++ ) {

      for ( int j = 7 ; j > 0 ; j-- )
        ShiftRegister[i][j] = ShiftRegister[i][j-1] ;

      ShiftRegister[i][0] = ReferenceBit[i] + ASC_ZERO ;
      ReferenceBit[i] = 0 ;

    } // for

    outfile << pageReference.at(current) << "\t" ;
    for ( int j = 0 ; j < pageFrame ; j++ ) {
      if ( page[j] != '\0' )
        outfile << page[j] ;
    } // for
    outfile << "\t" ;
    if ( bool_pageFault )
      outfile << "F" ;
    outfile << endl ;

  } // for

  outfile << "Page Fault = " << pageFault << "  Page Replaces = " << pageReplaces <<
             " Page Frames = " << pageFrame << endl ;

} // AdditionalReferenceBits

void SecondChance( string pageReference, int pageFrame, string filename ) {

  int pageFault = 0 ;
  int pageReplaces = 0 ;

  filename = "output_" + filename ;
  ofstream outfile( filename.c_str(), ios::app ) ;
  outfile << "--------------Second chance Page-------------------------" << endl ;

  char page[pageFrame] ;
  InitialArray( page, pageFrame ) ;
  int timeStamp[pageFrame] ;
  int getSecond[pageFrame] ;
  for ( int i = 0 ; i < pageFrame ; i++ ) { // initial time stamp array ;
    timeStamp[i] = 0 ;
    getSecond[i] = 0 ;
  } // for

  int timer = 0 ;

  for ( int current = 0 ; current < pageReference.size() ; current++, timer++ ) {

    int smallestTimeIndex = -1 ;
    int smallestTimeRecord = 9999 ;
    bool bool_pageFault = false ;
    for ( int j = 0 ; j < pageFrame ; j++ ) {
      // find the time stamp smallest to change ;

      if ( timeStamp[j] < smallestTimeRecord ) {
        smallestTimeRecord = timeStamp[j] ;
        smallestTimeIndex = j ;
      } // if

    } // for


    bool alreadyInPage = false ;
    for ( int j = 0 ; j < pageFrame ; j++ ) {
      // if already in page ;

      if ( page[j] == pageReference.at(current) ) {
        alreadyInPage = true ;
        getSecond[j] = 1 ;
        break ;
      } // if

    } // for

    if ( !alreadyInPage ) {
      pageFault++ ;
      bool_pageFault = true ;
      if ( pageFault > pageFrame )
        pageReplaces++ ;
    } // if

    int temppage = -1 ;
    bool getTheSecond = false ;
    for ( int j = pageFrame-1 ; !alreadyInPage && j >= 0 ; j-- ) {

      if ( j-1 >= 0 ) {

        if ( getSecond[j] == 1 && j == pageFrame-1 ) {
          getTheSecond = true ;
          temppage = page[j] ;
        } // if
        page[j] = page[j-1] ;
        page[j-1] = '\0' ;
        timeStamp[j] = timeStamp[j-1] ;
        timeStamp[j-1] = 0 ;

        getSecond[j] = getSecond[j-1] ;
        getSecond[j-1] = 0 ;
      } // if

      if ( j == 0 && getTheSecond ) {
        getTheSecond = false ;
        page[0] = temppage ;
        timeStamp[0] = timer ;
        getSecond[0] = 0 ;
        j = pageFrame ;
      } // if

    } // for

    if ( !alreadyInPage ) {
      page[0] = pageReference.at(current) ;
      timeStamp[0] = timer ;
      getSecond[0] = 1 ;
    } // if


    outfile << pageReference.at(current) << "\t" ;
    for ( int j = 0 ; j < pageFrame ; j++ ) {
      if ( page[j] != '\0' )
        outfile << page[j] ;
    } // for
    outfile << "\t" ;
    if ( bool_pageFault )
      outfile << "F" ;
    outfile << endl ;

  } // for

  outfile << "Page Fault = " << pageFault << "  Page Replaces = " << pageReplaces <<
             " Page Frames = " << pageFrame << endl ;

} // SecondChance

void LeastFrequentlyUsed( string pageReference, int pageFrame, string filename ) {

  int pageFault = 0 ;
  int pageReplaces = 0 ;
  char page[pageFrame] ;
  InitialArray( page, pageFrame ) ;
  filename = "output_" + filename ;
  ofstream outfile( filename.c_str(), ios::app ) ;
  outfile << "--------------Least Frequently Used Page Replacement --------------------" << endl ;

  int timeStamp[pageFrame] ;
  int counter[pageFrame] ;
  for ( int i = 0 ; i < pageFrame ; i++ ) { // initial time stamp array ;
    counter[i] = 0 ;
    timeStamp[i] = 0 ;
  } // for

  int timer = 0 ;

  for ( int current = 0 ; current < pageReference.size() ; current++, timer++ ) {

    int smallestTimeIndex = -1 ;
    int smallestTimeRecord = 9999 ;
    int temptime = 999 ;
    bool bool_pageFault = false ;
    for ( int j = 0 ; j < pageFrame ; j++ ) {
      // find the counter smallest to change ;

      if ( counter[j] < smallestTimeRecord ) {
        smallestTimeRecord = counter[j] ;
        smallestTimeIndex = j ;
        temptime = timeStamp[j] ;
      } // if
      else if ( counter[j] == smallestTimeRecord && timeStamp[j] < temptime ) {
        smallestTimeRecord = counter[j] ;
        smallestTimeIndex = j ;
        temptime = timeStamp[j] ;
      } // else if

    } // for

    bool alreadyInPage = false ;
    for ( int j = 0 ; j < pageFrame ; j++ ) {
      // if already in page ;

      if ( page[j] == pageReference.at(current) ) {
        alreadyInPage = true ;
        counter[j] = counter[j] + 1 ;
        timeStamp[j] = timer ;
        break ;
      } // if

    } // for

    if ( !alreadyInPage ) {
      pageFault++ ;
      bool_pageFault = true ;
      if ( pageFault > pageFrame )
        pageReplaces++ ;
    } // if

    int jsize = 0 ;
    if ( pageFault <= pageFrame )
      jsize = pageFrame - 1 ;
    else
      jsize = smallestTimeIndex ;

    for ( int j = jsize ; !alreadyInPage && j >= 0 ; j-- ) {

      if ( j-1 >= 0 ) {

        page[j] = page[j-1] ;
        page[j-1] = '\0' ;
        counter[j] = counter[j-1] ;
        counter[j-1] = 0 ;
        timeStamp[j] = timeStamp[j-1] ;
        timeStamp[j-1] = 0 ;

      } // if

    } // for

    if ( !alreadyInPage ) {
      page[0] = pageReference.at(current) ;
      counter[0] = 0 ;
      timeStamp[0] = timer ;
    } // if


    outfile << pageReference.at(current) << "\t" ;
    for ( int j = 0 ; j < pageFrame ; j++ ) {
      if ( page[j] != '\0' )
        outfile << page[j] ;
    } // for
    outfile << "\t" ;
    if ( bool_pageFault )
      outfile << "F" ;
    outfile << endl ;

  } // for

  outfile << "Page Fault = " << pageFault << "  Page Replaces = " << pageReplaces <<
             " Page Frames = " << pageFrame << endl ;

} // LeastFrequentlyUsed

void MostFrequentlyUsed( string pageReference, int pageFrame, string filename ) {

  int pageFault = 0 ;
  int pageReplaces = 0 ;
  char page[pageFrame] ;
  InitialArray( page, pageFrame ) ;
  filename = "output_" + filename ;
  ofstream outfile( filename.c_str(), ios::app ) ;
  outfile << "--------------Most Frequently Used Page Replacement ------------------------" << endl ;

  int timeStamp[pageFrame] ;
  int counter[pageFrame] ;
  for ( int i = 0 ; i < pageFrame ; i++ ) { // initial time stamp array ;
    counter[i] = 0 ;
    timeStamp[i] = 0 ;
  } // for

  int timer = 0 ;

  for ( int current = 0 ; current < pageReference.size() ; current++, timer++ ) {

    int LargestTimeIndex = -1 ;
    int LargestTimeRecord = 0 ;
    int temptime = 999 ;
    bool bool_pageFault = false ;
    for ( int j = 0 ; j < pageFrame ; j++ ) {
      // find the counter smallest to change ;

      if ( counter[j] > LargestTimeRecord ) {
        LargestTimeRecord = counter[j] ;
        LargestTimeIndex = j ;
        temptime = timeStamp[j] ;
      } // if
      else if ( counter[j] == LargestTimeRecord && timeStamp[j] < temptime ) {
        LargestTimeRecord = counter[j] ;
        LargestTimeIndex = j ;
        temptime = timeStamp[j] ;

      } // else if

    } // for

    bool alreadyInPage = false ;
    for ( int j = 0 ; j < pageFrame ; j++ ) {
      // if already in page ;

      if ( page[j] == pageReference.at(current) ) {
        alreadyInPage = true ;
        counter[j] = counter[j] + 1 ;
        timeStamp[j] = timer ;
        break ;
      } // if

    } // for

    if ( !alreadyInPage ) {
      pageFault++ ;
      bool_pageFault = true ;
      if ( pageFault > pageFrame )
        pageReplaces++ ;
    } // if

    int jsize = 0 ;
    if ( pageFault <= pageFrame )
      jsize = pageFrame - 1 ;
    else
      jsize = LargestTimeIndex ;

    for ( int j = jsize ; !alreadyInPage && j >= 0 ; j-- ) {

      if ( j-1 >= 0 ) {

        page[j] = page[j-1] ;
        page[j-1] = '\0' ;
        counter[j] = counter[j-1] ;
        counter[j-1] = 0 ;
        timeStamp[j] = timeStamp[j-1] ;
        timeStamp[j-1] = 0 ;

      } // if

    } // for

    if ( !alreadyInPage ) {
      page[0] = pageReference.at(current) ;
      counter[0] = 0 ;
      timeStamp[0] = timer ;
    } // if


    outfile << pageReference.at(current) << "\t" ;
    for ( int j = 0 ; j < pageFrame ; j++ ) {
      if ( page[j] != '\0' )
        outfile << page[j] ;
    } // for
    outfile << "\t" ;
    if ( bool_pageFault )
      outfile << "F" ;
    outfile << endl ;

  } // for

  outfile << "Page Fault = " << pageFault << "  Page Replaces = " << pageReplaces <<
             " Page Frames = " << pageFrame << endl ;

} // MostFrequentlyUsed

void InitialArray( char charray[], int pageFrame ) {

  for ( int i = 0 ; i < pageFrame ; i++ )
    charray[i] = '\0' ;

} // InitialArray

string getInput( string filename, int & pageFrame ) {

	fstream inFile ;
	char buffer[255] ;

	inFile.open( filename.c_str(), fstream::in ) ; // open file ;

	inFile.getline( buffer, 255, '\n' ) ;
	string strInput ;

	strInput.assign( buffer ) ;
	for ( int i = 0 ; i < strInput.size() ; i++ ) {

    pageFrame = pageFrame * 10 + ( strInput.at(i) - ASC_ZERO ) ;

	} // for

  strInput.clear() ;
	inFile.getline( buffer, 255, '\n' ) ; // skip the name line ;

	strInput.assign( buffer ) ;

	inFile.close() ;

	return strInput ;

} // getInput()
