
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "seriallink.h"
#include<QtSql>
#include<QtDebug>
#include<QFileInfo>
#include <QTimer>
#include <QDate>
#include <QDebug>
#include <QtWidgets>
#include <QSerialPort>
#include<QtCharts>
#include<QChartView>
#include<QLineSeries>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QSqlDatabase mydb;
    void connClose(){
        mydb.close();
        mydb.removeDatabase(QSqlDatabase::defaultConnection);

    }
    bool connOpen(){
        mydb=QSqlDatabase::addDatabase("QSQLITE");
        mydb.setDatabaseName("C:/Users/omayma/Desktop/PFE-VF-QT/PROJET_DJIN_1/DJIN_1.db");
//                QString path = QCoreApplication::applicationDirPath() + "/DJIN_1.db";
//                mydb.setDatabaseName(path);

        if(!mydb.open()){
            qDebug()<<("Failed to open the database");
            return false;
        }
        else{
            qDebug()<<("Connected...");
            return true;
        }
    }
public slots :
//    void updateGUI(QByteArray data);
    void heure ();

private slots :
    void hover_button ();
    void on_START_clicked();
    void get_username_and_password_from_login();
    void on_LOGIN_2_clicked();
    void on_password_2_returnPressed();
    void on_Logout_clicked();
    void on_Stockage_bouton_clicked();
    void on_return_page2_from_stockage_clicked();

    void ouvrir_tiroir();
    void on_Valider_stockage_clicked();
    void tab_nbr_niveau_tiroir();
    void on_comboBox_logout_currentIndexChanged(int index);

    void on_suivant_config_clicked();

    void on_return_page2_from_config_clicked();


    void on_Inventaire_bouton_clicked();


    void on_return_page2_from_inventaire_clicked();

    void on_historique_bouton_clicked();

    void on_utilisateur_returnPressed();

    void on_return_page2_from_historique_clicked();

    void supprimer_tab_etat_armoire();
    void supprimer_tab_armoire();

    void on_valider_config_3_clicked();

    void on_Ordonnance_bouton_clicked();

    void on_visualiser_pour_creer_ordonnance_clicked();

    void on_creer_bouton_pour_creer_ordonnance_clicked();

    void on_return_page2_from_ordonnance_clicked();

    void on_Distribution_bouton_clicked();

    void on_tableau_liste_des_ordonnance_clicked(const QModelIndex &index);

    void on_return_page2_from_distribution_clicked();

    void on_Valider_3_clicked();

    void on_ouvrir_clicked();

    void on_return_page4_from_empl_med_clicked();

    void on_stockage_indiv_clicked();

    void on_activite_currentIndexChanged(int index);

    void on_date_hist_dateChanged(const QDate &date);

    void creation_tableau_liste_des_utilisateurs();
    void on_bouton_liste_des_utilisateurs_clicked();

    void on_bouton_ajouter_utilisateur_clicked();

    void on_Utilisateurs_bouton_clicked();

    void on_return_page2_from_historique_2_clicked();


    void on_confirmer_nouvel_utilisateur_clicked();


    void on_role_currentIndexChanged(int index);
    void on_remove_clicked();

    void on_comboBox_2_currentIndexChanged(int index);
    void creation_label_armoire();
    void on_modify_clicked();


private:
    std::vector<QLabel*> labels;
    std::vector<QLabel*> labels_armoir;
    seriallink *arduino;
    std::list<std::string> tiroir_vide_pour_distribution_List;
    QList<QString> compartiments;
    QList<int> troisPremiersInt;
    QList<int>PremiersInt,PremiersInt_non;

    QDate *date;
    QTimer *timer;
    QTime *time;
    QLCDNumber *afficheur1,*afficheur2;
    int id_empreinte,T,var,id_ord,num,row = 0,tiroir_distribution,niv_tir_stock,compartimentInt, clickCount=0,compdisp,  maxValue = 0,dernierElementInt,dernierElementInt_stockage,dernierElementInt_stockage_non;
    QString niveauIntt;
    QLabel* messageLabel_ord;
     std::string dernierNiveau,premierTiroir,Niveau_pour_distribution;
    QString datetext,time_text,user,role,password_login,med_dans_tab_stockage,quantite_dans_tab_stockage
        ,tiroir,numberStr,ID,medicament1,medicament2,medicament3,quantite1,quantite2,quantite3,tir_1,tir_2,
        tir_3,niv1,niv2,niv3,niv4,niv5,numero_armoire,nbr_tir_dans_armoire,
        numero_tiroir,nombre_de_compartiment,nbr_niv_tiroir1,quantit,
        nbr_niv_tiroir2,nbr_niv_tiroir3,nbr_niv_tiroir4,nbr_niv_tiroir5,niveau_med,tiroir_stockage,
        num_tiroir,nbr_compartiment,med_et_quantit,medicament1_ord,medicament2_ord,medicament3_ord,
        quantite1_ord,quantite2_ord,quantite3_ord,patient_ord,id_ordonnance_selectionne,meds
        ,selectedItem,tiroir_pour_distribution,tiroir_med_distribution,num_tiroir_stockage,numero_de_tiroir_pour_stockage
    ,fonct_user,type_activite,num_niv_tiroir_stockage,nbre_compartiment,niveau_med_dist,med_distribution,nom_utilisateur_line_edit,mot_de_passe_line_edit
    ,fonction_line_edit,medecin_line_edit,id_new_user,quantite_med_distribution,quantite_inventaire_med,tiroir_med_dist
    ,med_pour_stockage,tiroir_stockage_med_existant,premierTiroir_vide,med_dans_tab_distribuion,qt_dans_tab_distribution,
    niveau_vide_distribution,premierNiveau_vide,niveau_tir_stockage,med_a_stocker,tiroir_de_stockage,niveau_de_stockage,username_for_delete,ID_username,compartimentStr,nbr_compartiment_stock,dernierElement;
    QStandardItemModel *model_pour_tableau_de_stockage,*model_pour_tableau_configuration_tiroir, *inventaire_tab_model, *model_pour_tableau_historique,*model_nom_medicament_pour_creer_ordonnance
        ,*med_qt_ord_tab_model,*ord_list_model,*modif_ord_model,*model7,*med_model_tab,*model_pour_medicament_tab
    ,*model_pour_liste_tiroir_vide,*historique_tab,*model_nombre_de_compartiments,*model_tableau_utilisateur,*medicament_qt_et_ouvrir_model,*med_qt_ord_selectionne_tab_model;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
