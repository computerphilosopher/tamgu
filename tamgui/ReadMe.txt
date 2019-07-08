================================================================================
    BIBLIOTH�QUE MICROSOFT FOUNDATION CLASS : Vue d'ensemble du projet 
    TAMGUI
================================================================================

L'Assistant Application a cr�� cette application TAMGUI pour 
vous.  Cette application d�crit les principes de base de l'utilisation de 
Microsoft Foundation Classes et vous permet de cr�er votre application.

Ce fichier contient un r�sum� du contenu de chacun des fichiers qui constituent 
votre application TAMGUI.

TAMGUI.vcxproj
    Il s'agit du fichier projet principal pour les projets VC++ g�n�r�s � l'aide 
    d'un Assistant Application. 
    Il contient les informations sur la version de Visual C++ qui a g�n�r� le 
    fichier et des informations sur les plates-formes, configurations et 
    fonctionnalit�s du projet s�lectionn�es avec l'Assistant Application.

TAMGUI.vcxproj.filters
    Il s'agit du fichier de filtres pour les projets VC++ g�n�r�s � l'aide d'un 
    Assistant Application. 
    Il contient des informations sur l'association entre les fichiers de votre 
    projet et les filtres. Cette association est utilis�e dans l'IDE pour 
    afficher le regroupement des fichiers qui ont des extensions similares sous 
    un n�ud sp�cifique (par exemple, les fichiers ".cpp" sont associ�s au 
    filtre "Fichiers sources").

TAMGUI.h
    Il s'agit du fichier d'en-t�te principal de l'application.  Il contient 
    d'autres en-t�tes de projet sp�cifiques (y compris Resource.h) et d�clare 
    la classe d'application CTAMGUIApp.

TAMGUI.cpp
    Il s'agit du fichier source principal de l'application qui contient la 
    classe d'application CTAMGUIApp.

TAMGUI.rc
    Il s'agit de la liste de toutes les ressources Microsoft Windows que le 
    programme utilise.  Elle comprend les ic�nes, les bitmaps et les curseurs 
    qui sont stock�s dans le sous-r�pertoire RES. Ce fichier peut �tre modifi� 
    directement dans Microsoft Visual C++. Vos ressources de projet sont dans 
    1036.

res\TAMGUI.ico
    Il s'agit d'un fichier ic�ne, qui est utilis� comme ic�ne de l'application.  
    Cette ic�ne est incluse par le fichier de ressource principal 
    TAMGUI.rc.

res\TAMGUI.rc2
    Ce fichier contient les ressources qui ne sont pas modifi�es par Microsoft  
    Visual C++. Vous devez placer toutes les ressources non modifiables par 
    l'�diteur de ressources dans ce fichier.

TAMGUI.reg
    Il s'agit d'un exemple de fichier .reg qui montre le type de param�tres 
    d'enregistrement que le framework d�finit pour vous.  Vous pouvez l'utiliser 
    comme fichier .reg
    pour votre application ou le supprimer et utiliser  
    l'enregistrement par d�faut RegisterShellFileTypes.


/////////////////////////////////////////////////////////////////////////////

Pour la fen�tre frame principale :
    Le projet comprend une interface MFC standard.

MainFrm.h, MainFrm.cpp
    Ces fichiers contiennent la classe de frame CMainFrame 
    d�riv�e de
    CMDIFrameWnd et qui contr�le toutes les fonctionnalit�s des frames MDI.

/////////////////////////////////////////////////////////////////////////////

Pour la fen�tre frame enfant :

ChildFrm.h, ChildFrm.cpp
    Ces fichiers d�finissent et impl�mentent la classe CChildFrame 
    qui prend en charge les fen�tres enfants d'une application MDI.

/////////////////////////////////////////////////////////////////////////////

L'Assistant Application cr�e un type de document et une vue :

TAMGUIDoc.h, TAMGUIDoc.cpp - le document
    Ces fichiers contiennent votre classe CTAMGUIDoc.  Modifiez ces 
fichiers pour 
    ajouter les donn�es de document sp�ciales et impl�menter l'enregistrement 
    et le chargement des fichiers (via CTAMGUIDoc::Serialize).
    Le document contiendra les cha�nes suivantes :
        Extension de fichier :         tamgu
        ID du type de fichier :        TAMGUI.Document
        Titre du frame principal :     TAMGUI
        Nom du type de document :      TAMGUI
        Nom de filtre :                Fichiers Tamgu (*.tmg)
        Nom court de nouveau fichier : TAMGUI
        Nom long du type de fichier :  TAMGUI.Document

TAMGUIView.h, TAMGUIView.cpp - la vue du document
    Ces fichiers contiennent votre classe CTAMGUIView.
    Les objets CTAMGUIView servent � afficher les objets 
    CTAMGUIDoc.

res\TAMGUIDoc.ico
    Il s'agit d'un fichier ic�ne, qui est utilis� comme ic�ne des fen�tres 
    enfants MDI pour la classe CTAMGUIDoc.  Cette ic�ne est incluse 
    par le fichier de ressources principal TAMGUI.rc.




/////////////////////////////////////////////////////////////////////////////

Autres fonctionnalit�s :

Contr�les ActiveX
    L'application comprend la prise en charge des contr�les ActiveX.

Prise en charge de l'impression et de l'aper�u avant impression
    L'Assistant Application a g�n�r� un code pour g�rer les commandes 
    d'impression, de configuration de l'impression et d'aper�u avant 
    impression en appelant les fonctions membres dans la classe CView, � 
    partir de la biblioth�que MFC.

/////////////////////////////////////////////////////////////////////////////

Autres fichiers standard :

StdAfx.h, StdAfx.cpp
    Ces fichiers sont utilis�s pour g�n�rer un fichier d'en-t�te pr�compil� 
    (PCH) nomm� TAMGUI.pch et un fichier de type pr�compil� 
    nomm� Stdafx.obj.

Resource.h
    Il s'agit du ficher d'en-t�te standard, qui d�finit les nouveaux ID de 
    ressources.
    Microsoft Visual C++ lit et met � jour ce fichier.

TAMGUI.manifest
    Les fichiers manifestes d'application sont utilis�s par Windows XP pour 
    d�crire les d�pendances des applications sur des versions sp�cifiques 
    des assemblys c�te � c�te. Le chargeur utilise ces informations pour 
    charger l'assembly appropri� � partir du cache de l'assembly ou 
    directement � partir de l'application. Le manifeste de l'application peut 
    �tre inclus pour redistribution comme fichier .manifest externe install� 
    dans le m�me dossier que l'ex�cutable de l'application ou �tre inclus 
    dans l'ex�cutable sous la forme d'une ressource. 
/////////////////////////////////////////////////////////////////////////////

Autres remarques :

L'Assistant Application utilise "TODO:" pour indiquer les parties du code 
source o� vous devrez ajouter ou modifier du code.

Si votre application utilise les MFC dans une DLL partag�e vous devez 
redistribuer les DLL MFC. Si la langue de votre application n'est pas celle 
du syst�me d'exploitation, vous devez �galement redistribuer le fichier des 
ressources localis�es MFC100XXX.DLL. Pour plus d'informations, consultez la 
section relative � la redistribution des applications Visual C++ dans la 
documentation MSDN.

/////////////////////////////////////////////////////////////////////////////
