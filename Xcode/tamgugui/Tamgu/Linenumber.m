/*
 *  Tamgu (탐구)
 *
 * Copyright 2019-present NAVER Corp.
 * under BSD 3-clause
 * This file can only be used with the Tamgu (탐구) library or the executable
 * it should not and cannot be used otherwise.
 */
/* --- CONTENTS ---
 Project    : Tamgu (탐구)
 Version    : See tamgu.cxx for the version number
 filename   : Linenumber.m
 Date       : 2017/09/01
 Purpose    : 
 Programmer : Claude ROUX (claude.roux@naverlabs.com)
 Reviewer   :
 */

void addabreakpoint(const char* filename, long numline, char add);
void clearallbreakpoints(void);


#import "Linenumber.h"

@implementation Linenumber


-(void)ajoutey:(NSInteger)i {
    if (ypositions==NULL)
        ypositions=[[NSMutableArray alloc] init] ;
    
    [ypositions addObject:[NSNumber numberWithInteger:i+3]];
}

-(void)nettoiey {
    if (ypositions==NULL)
        return;
    [ypositions removeAllObjects];
}

-(void)ajoutel:(NSInteger)i {
    if (lignes==NULL)
        lignes=[[NSMutableArray alloc] init];
    
    [lignes addObject:[NSNumber numberWithInteger:i]];
}

-(void)nettoiel {
    if (lignes==NULL)
        return;
    [lignes removeAllObjects];
}


- (id)init
{
    self = [super init];
    if (self) {
        
    }
    return self;
}

-(void)Ligne:(NSMutableArray*)l {
    for (NSNumber* n in lignes)
        [l addObject:n];
}

-(NSWindow*)fenetre {
    return [[self clientView] window];
}

- (void)selectionne:(long)ligne {
    if (ligne < 0 || ligne >= [lignes count])
        return;
        
    long debut=[[lignes objectAtIndex:ligne-1] intValue];
    long fin;
    NSTextView* textecode= (NSTextView*)[self clientView];
    if (ligne < [lignes count])
        fin=[[lignes objectAtIndex:ligne] intValue];
    else
        fin=[[textecode textStorage] length];
    
    NSRange rg=NSMakeRange(debut, fin-debut);
    [textecode setSelectedRange:rg];
    [textecode scrollRangeToVisible: rg];
}

-(bool)findbreakline:(int)ligne {
    if (breakpoints==NULL)
        return false;
    long idx=[breakpoints indexOfObjectIdenticalTo:[NSNumber numberWithLong:ligne]];
    if (idx==NSNotFound)
        return false;
    return true;
}

-(void)clearbreaks {
    [breakpoints removeAllObjects];
}

-(long)addbreak:(NSInteger)pos {
    if (breakpoints==NULL)
        breakpoints=[[NSMutableArray alloc] init];
    
    const char* nom=[[[[self clientView] window] title] UTF8String];
    
    long ligne=0;
    for (NSNumber* n in lignes)  {
        if (pos<[n intValue])
            break;
        ligne++;
    }
    
    long idx=[breakpoints indexOfObjectIdenticalTo:[NSNumber numberWithLong:ligne]];
    if (idx!=NSNotFound) {
        //We remove it
        [breakpoints removeObjectAtIndex:idx];
        addabreakpoint(nom,ligne,0);
    }
    else {
        [breakpoints addObject:[NSNumber numberWithLong:ligne]];
        addabreakpoint(nom,ligne,1);
    }
    
    return ligne;
}

- (void)numerolignes {
    
    NSTextView* textecode=(NSTextView*)[self clientView];
    NSScrollView* lecode=[self scrollView];
    NSLayoutManager            *layoutManager;
    NSTextContainer            *container;
    NSRect                    visibleRect;
    
    layoutManager = [textecode layoutManager];
    container = [textecode textContainer];
    visibleRect = [[lecode contentView] bounds];
    //NSRange glyphRange = [layoutManager glyphRangeForBoundingRect:visibleRect inTextContainer:container];
    //NSRange range = [layoutManager characterRangeForGlyphRange:glyphRange actualGlyphRange:NULL];
    
    int l=0;
    NSString* labelText;
    NSSize szn;
    int base=visibleRect.origin.y;
    long mn=0;
    for (NSNumber* n in lignes)  {
        // if ([n intValue]>=range.location) { //&& [n intValue]<=(range.location+range.length)) {
        labelText = [NSString stringWithFormat:@"%d", l+1];
        
        if ([self findbreakline:l+1]) {
            szn=[labelText sizeWithAttributes: attributsBreaks];
            
            NSRect rectb=NSMakeRect(3,[[ypositions objectAtIndex:l] intValue]-base,
                                    szn.width+2, szn.height);
            // Draw string flush right, centered vertically within the line
            [labelText drawInRect:rectb withAttributes:attributsBreaks];
            
        }
        else {
            szn=[labelText sizeWithAttributes: attributsNumeros];
            
            NSRect rectb=NSMakeRect(3,[[ypositions objectAtIndex:l] intValue]-base,
                                    szn.width+2, szn.height);
            // Draw string flush right, centered vertically within the line
            [labelText drawInRect:rectb withAttributes:attributsNumeros];
        }
        //}
        mn=[n intValue]+1;
        l++;
    }
}

- (void)drawHashMarksAndLabelsInRect:(NSRect)rect {
    
    if (attributsNumeros==NULL) {
        attributsNumeros=[NSDictionary dictionaryWithObjectsAndKeys:
                          [NSFont fontWithName:@"Helvetica" size:10.0], NSFontAttributeName,
                          [NSColor lightGrayColor], NSForegroundColorAttributeName,
                          nil];
        NSFontManager *fontManager = [NSFontManager sharedFontManager];
        NSFont *italique = [fontManager fontWithFamily:@"Helvetica"
                                                traits:NSItalicFontMask
                                                weight:0
                                                  size:10];
        attributsBreaks=[NSDictionary dictionaryWithObjectsAndKeys:
                         italique, NSFontAttributeName,
                         [NSColor blueColor], NSForegroundColorAttributeName,
                         nil];
    }
    
    [self numerolignes];
    
}

@end
