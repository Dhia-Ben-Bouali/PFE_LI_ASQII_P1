#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QMessageBox>
#include <QtCore>
#include<QComboBox>
#include<QtGui>
#include <QTimer>
#include <QDate>
#include <QDebug>
#include <QPixmap>
#include <QDebug>
#include <QtWidgets>
#include <QKeyEvent>
#include <QDialog>
#include <QVBoxLayout>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QRegularExpression>
#include<QtCharts>
#include<QChartView>
#include<QLineSeries>
#include <QTimer>
#include <iostream>
#include <QLCDNumber>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    ,model_pour_tableau_de_stockage(new QStandardItemModel(1,3, this))
    ,model_pour_tableau_configuration_tiroir(new QStandardItemModel(0,4, this))
    ,inventaire_tab_model(new QStandardItemModel(0,4, this))
    ,model_pour_tableau_historique(new QStandardItemModel(0,17, this))
    ,model_nom_medicament_pour_creer_ordonnance(new QStandardItemModel())
    ,med_qt_ord_tab_model(new QStandardItemModel(0,3,this))
    ,ord_list_model(new QStandardItemModel(0,0))
    ,model7(new QStandardItemModel())
    ,med_model_tab(new QStandardItemModel(0,3, this))
    ,model_pour_medicament_tab(new QStandardItemModel(0,3, this))
    ,model_pour_liste_tiroir_vide(new QStandardItemModel())
    ,historique_tab(new QStandardItemModel(0,8, this))
    ,model_nombre_de_compartiments(new QStandardItemModel())
    ,model_tableau_utilisateur(new QStandardItemModel(0,6, this))
    ,medicament_qt_et_ouvrir_model(new QStandardItemModel(0,3, this))
    ,med_qt_ord_selectionne_tab_model(new QStandardItemModel(0,2, this))

    , ui(new Ui::MainWindow)

{
    arduino = new seriallink;
    arduino->openConnection();
//        connect(arduino, &seriallink::gotNewData, this, &MainWindow::updateGUI);

      connOpen();
      ui->setupUi(this);
//      connect(model_pour_tableau_de_stockage, &QAbstractItemModel::dataChanged, this, &MainWindow::on_data_changed_tableau_stockage);

      timer = new QTimer(this);
      connect(timer,SIGNAL(timeout()),this,SLOT(heure()));
      timer->start();
      QDate date= QDate::currentDate();
      datetext=date.toString("dd/MM/yyyy");


      ui->date_1->setText(datetext);
      ui->date_3->setText(datetext);
      ui->date_7->setText(datetext);
      ui->date_17->setText(datetext);
      ui->date_pour_creer_ordonnance->setDate(date);
      ui->date_hist->setDate(date);
      ui->date_4->setText(datetext);
      ui->date_18->setText(datetext);
      ui->date_19->setText(datetext);
      ui->date_20->setText(datetext);
      ui->date_21->setText(datetext);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::heure()
{
    QTime time = QTime::currentTime();
    time_text = time.toString("hh : mm : ss");

    ui->heure_1->setText(time_text);
    ui->heure_4->setText(time_text);
    ui->heure_17->setText(time_text);
    ui->heure_7->setText(time_text);
    ui->heure_18->setText(time_text);
    ui->heure_19->setText(time_text);
    ui->heure_20->setText(time_text);
    ui->heure_5->setText(time_text);
    ui->heure_21->setText(time_text);
}
void MainWindow::hover_button ()
{

    ui->Ordonnance_bouton->setStyleSheet("QPushButton {"

                                         "     color: #26326A;"
                                         "     background-color:  #E9EEF9;"
                                         "border-radius: 8.85963px;"


                                         "}"
                                         "QPushButton:hover {"
                                         "     background-color: #0096DC;"
                                         "     color: white;"

                                         "}");


    ui->Distribution_bouton->setStyleSheet("QPushButton {"

                                           "     color: #26326A;"
                                           "     background-color:  #E9EEF9;"
                                           "border-radius: 8.85963px;"


                                           "}"
                                           "QPushButton:hover {"
                                           "     background-color: #0096DC;"
                                           "     color: white;"

                                           "}");

    ui->Stockage_bouton->setStyleSheet("QPushButton {"

                                       "     color: #26326A;"
                                       "     background-color:  #E9EEF9;"
                                       "border-radius: 8.85963px;"


                                       "}"
                                       "QPushButton:hover {"
                                       "     background-color: #0096DC;"
                                       "     color: white;"

                                       "}");
    ui->Inventaire_bouton->setStyleSheet("QPushButton {"

                                         "     color: #26326A;"
                                         "     background-color:  #E9EEF9;"
                                         "border-radius: 8.85963px;"


                                         "}"
                                         "QPushButton:hover {"
                                         "     background-color: #0096DC;"
                                         "     color: white;"

                                         "}");
    ui->historique_bouton->setStyleSheet("QPushButton {"

                                         "     color: #26326A;"
                                         "     background-color:  #E9EEF9;"
                                         "border-radius: 8.85963px;"


                                         "}"
                                         "QPushButton:hover {"
                                         "     background-color: #0096DC;"
                                         "     color: white;"

                                         "}");
    ui->Utilisateurs_bouton->setStyleSheet("QPushButton {"

                                           "     color: #26326A;"
                                           "     background-color:  #E9EEF9;"
                                           "border-radius: 8.85963px;"


                                           "}"
                                           "QPushButton:hover {"
                                           "     background-color: #0096DC;"
                                           "     color: white;"

                                           "}");
}
void MainWindow::on_START_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

}
////////////////////////////////////////////////////////////////////******* fonction mtaa login  ************
void MainWindow::get_username_and_password_from_login()
{
    QString username,password;
    username=ui->username_2->text();
    password=ui->password_2->text();

    if(!connOpen())
    {
        qDebug()<<"Failed to open the Database";
        return;
    }

    QSqlQuery get_username_and_password_from_login;
    get_username_and_password_from_login.prepare("SELECT * FROM USER WHERE USERNAME LIKE :username AND PASSWORD = :password");
    get_username_and_password_from_login.bindValue(":username", username);
    get_username_and_password_from_login.bindValue(":password", password);


    if (!get_username_and_password_from_login.exec()) {
        qDebug() << "Failed to execute query login";
        qDebug() << get_username_and_password_from_login.lastError().text();

    }
    int count=0;
    while (get_username_and_password_from_login.next()) {
        user=get_username_and_password_from_login.value("USERNAME").toString();
        role=get_username_and_password_from_login.value("ROLE").toString();

        count++;
    }
    if(count==1)
    {

        password_login=ui->password_2->text();

        connClose();
        ui->stackedWidget->setCurrentIndex(2);

        ui->Logout->setText(user);
        ui->nom_log->setText(user);
        ui->nom_log_1->setText(user);
        ui->nom_log_4->setText(user);
        ui->nom_log_5->setText(user);
        ui->nom_log_6->setText(user);
        ui->nom_log_7->setText(user);
        ui->nom_log_8->setText(user);

        if (role=="Docteur")
        {
            ui->rectangle_user_3->setGeometry(361,120,759,531);
            ui->dashboard->setGeometry(644,148,271,39);
            ui->Ordonnance_bouton->show();
            ui->ordonnance_label->show();
            ui->Distribution_bouton->show();
            ui->distribution_label->show();
            ui->Inventaire_bouton->show();
            ui->inventaire_label->show();
            ui->historique_bouton->show();
            ui->historique_label->show();
            ui->Stockage_bouton->hide();
            ui->stockage_label->hide();
            ui->Utilisateurs_bouton->hide();
            ui->utilisateur_label->hide();
            ui->Ordonnance_bouton->setGeometry(493,219,205,205);
            ui->ordonnance_label->setGeometry(493,384,205,31);
            ui->Distribution_bouton->setGeometry(781,219,205,205);
            ui->distribution_label->setGeometry(781,384,205,31);
            ui->Inventaire_bouton->setGeometry(493,440,205,205);
            ui->inventaire_label->setGeometry(493,605,205,31);
            ui->historique_bouton->setGeometry(781,440,205,205);
            ui->historique_label->setGeometry(781,605,205,31);
        }
        if(role=="Infirmier")
        {
            ui->rectangle_user_3->setGeometry(361,190,759,380);
            ui->Utilisateurs_bouton->hide();
            ui->stockage_label->hide();
            ui->utilisateur_label->hide();
            ui->historique_label->hide();
            ui->ordonnance_label->hide();
            ui->historique_bouton->hide();
            ui->Stockage_bouton->hide();
            ui->Ordonnance_bouton->hide();
            ui->Distribution_bouton->show();
            ui->distribution_label->show();
            ui->dashboard->setGeometry(605,221,271,39);
            ui->Distribution_bouton->setGeometry(493,290,205,205);
            ui->distribution_label->setGeometry(493,455,205,31);
            ui->Inventaire_bouton->setGeometry(781,290,205,205);
            ui->inventaire_label->setGeometry(781,455,205,31);

        }
        if(role=="Pharmacien")
        {
            ui->rectangle_user_3->setGeometry(361,190,759,380);
            ui->Utilisateurs_bouton->hide();
            ui->utilisateur_label->hide();
            ui->Ordonnance_bouton->hide();
            ui->ordonnance_label->hide();
            ui->historique_bouton->hide();
            ui->historique_label->hide();
            ui->Distribution_bouton->hide();
            ui->distribution_label->hide();
            ui->Stockage_bouton->show();
            ui->stockage_label->show();
            ui->dashboard->setGeometry(605,221,271,39);
            ui->Stockage_bouton->setGeometry(493,290,205,205);
            ui->stockage_label->setGeometry(493,455,205,31);
            ui->Inventaire_bouton->setGeometry(781,290,205,205);
            ui->inventaire_label->setGeometry(781,455,205,31);
        }

    if(role=="Admin")
    {
        ui->rectangle_user_3->setGeometry(250,120,981,531);
            ui->dashboard->show();
        ui->Ordonnance_bouton->show();
        ui->ordonnance_label->show();
        ui->Distribution_bouton->show();
        ui->distribution_label->show();
        ui->Stockage_bouton->show();
        ui->stockage_label->show();
        ui->Inventaire_bouton->show();
        ui->inventaire_label->show();
        ui->historique_bouton->show();
        ui->historique_label->show();
        ui->Utilisateurs_bouton->show();
        ui->utilisateur_label->show();
        ui->dashboard->setGeometry(644,148,271,39);
        ui->Ordonnance_bouton->setGeometry(408,219,181,181);
        ui->ordonnance_label->setGeometry(408,360,181,31);
        ui->Distribution_bouton->setGeometry(649,219,181,181);
        ui->distribution_label->setGeometry(649,360,181,31);
        ui->Stockage_bouton->setGeometry(890,219,181,181);
        ui->stockage_label->setGeometry(890,360,181,31);
        ui->Inventaire_bouton->setGeometry(408,440,181,181);
        ui->inventaire_label->setGeometry(408,581,181,31);
        ui->historique_bouton->setGeometry(650,440,181,181);
        ui->historique_label->setGeometry(650,581,181,31);
        ui->Utilisateurs_bouton->setGeometry(890,440,181,181);
        ui->utilisateur_label->setGeometry(890,581,181,31);
    }
    }
    if(count>1)
        ui->Message_login_2->setText("Duplicate username and password ...");
    if(count<1)
        ui->Message_login_2->setText("Nom d'utilisateur ou mot de passe incorrect ...");

    ui->username_2->clear();
    ui->password_2->clear();
}
////////////////////////////////////////////////////////////////////******* bouton mtaa login  ************

void MainWindow::on_LOGIN_2_clicked()
{   ui->Message_login_2->clear();
    get_username_and_password_from_login();
    hover_button();
    ui->comboBox_logout->setCurrentIndex(-1);
}
////////////////////////////////////////////////////////////////////******* ki nenzel entrer f  login  ************

void MainWindow::on_password_2_returnPressed()
{
    on_LOGIN_2_clicked();
}

////////////////////////////////////////////////////////////////////******* logout win maktoub l esm  ************

void MainWindow::on_Logout_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
////////////////////////////////////////////////////////////////////******* logout w parametre taa armoire ************

void MainWindow::on_comboBox_logout_currentIndexChanged(int index)
{
QString type_combobox = ui->comboBox_logout->currentText();
if (type_combobox=="Logout")
{
    ui->stackedWidget->setCurrentIndex(1);
}
if (type_combobox=="Parametres")
{
    ui->stackedWidget->setCurrentIndex(12);
    tab_nbr_niveau_tiroir();
    ui->tabWidget->setTabText(0,"Armoire");
    ui->tabWidget->setTabText(1,"Compartiments");
    ui->tabWidget->setCurrentIndex(0);
    ui->numero_armoire->text().clear();
    ui->nbr_tiroir->text().clear();
    ui->nombre_de_compartiment->text().clear();
}
}
////////////////////////////////////////////////////////////////////******* ki nenzel aal bouton prescription ************

void MainWindow::on_Ordonnance_bouton_clicked()
{
ui->stackedWidget->setCurrentIndex(3);
ui->medecin_pour_creer_ordonnance->setText(user);
med_qt_ord_tab_model->removeRows(0,med_qt_ord_tab_model->rowCount());
ui->patient_affichage_pour_creer_ordonnance->clear();;
ui->date_affichage_pour_creer_ordonnance->clear();
ui->patient_pour_creer_ordonnance->clear();
ui->meds_pour_creer_ordonnance->clear();
ui->qt_pour_creer_ordonnance->clear();
model_nom_medicament_pour_creer_ordonnance->clear();
ui->creer_bouton_pour_creer_ordonnance->hide();
ui->nom_user_affichage_pour_creer_ordonnance->hide();
ui->label_24->hide();
ui->patient_affichage_pour_creer_ordonnance->hide();
ui->label_25->hide();
ui->med_qt_ord_tab_pour_creer_ordonnance->hide();
ui->label_26->hide();
ui->date_affichage_pour_creer_ordonnance->hide();
ui->label_27->hide();
messageLabel_ord = ui->visual_ord_pour_creer_ordonnance;

ui->visual_ord_pour_creer_ordonnance->setText("Veuillez créer votre ordonnance");
    ui->visual_ord_pour_creer_ordonnance->setAlignment(Qt::AlignCenter);

if(!connOpen())
{
    qDebug()<<"Failed to open the Database";
    return;
}

QSqlQuery meds;
meds.exec("select NOM from MED ");

while (meds.next())
{
    QString name_meds = meds.value("NOM").toString();
    QStandardItem *item = new QStandardItem(name_meds);
    model_nom_medicament_pour_creer_ordonnance->appendRow(item);
}
ui->meds_pour_creer_ordonnance->setModel(model_nom_medicament_pour_creer_ordonnance);
connClose();
}
////////////////////////////////////////////////////////////////////******* ki nenzel aal bouton visualiser mtaa prescription ************

void MainWindow::on_visualiser_pour_creer_ordonnance_clicked()
{
med_qt_ord_tab_model->removeRows(0,med_qt_ord_tab_model->rowCount());

QString  patient1, medica,dateord;
ui->nom_user_affichage_pour_creer_ordonnance->setText(user);
patient1=ui->patient_pour_creer_ordonnance->text();
ui->patient_affichage_pour_creer_ordonnance->setText(patient1);
medica=ui->meds_pour_creer_ordonnance->currentText();
quantit=ui->qt_pour_creer_ordonnance->text();
med_et_quantit=medica+" :"+"quantité "+quantit;
dateord=ui->date_pour_creer_ordonnance->text();
ui->date_affichage_pour_creer_ordonnance->setText(dateord);
///////////////////////////////////////////////////////////////////// nseti l headers mtaa tableau visualisation prescription
med_qt_ord_tab_model->setHeaderData(0, Qt::Horizontal, "Medicament");
med_qt_ord_tab_model->setHeaderData(1, Qt::Horizontal, "Quantité");
    QScrollBar *scroll_bar = ui->med_qt_ord_tab_pour_creer_ordonnance->verticalScrollBar();
scroll_bar->setStyleSheet(
    "QScrollBar:vertical {"
    "width: 15px;"
    "margin: 15px 0 15px 0;"
    "}"
    "QScrollBar::handle:vertical {"
    "min-height: 20px;"
    "}"
    "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
    "height: 15px;"
    "width: 15px;"
    "}"
    "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
    "background: none;"
    "}"
    );
////////////////////////////////////////////////// nlawej ken l medicament mawjoud walé f inventaire bech najem nhotou f ordonnance
if(!connOpen())
{
    qDebug()<<"Failed to open the Database";
    return;
}
QSqlQuery check_meds;
check_meds.prepare("SELECT COUNT(*) FROM INVENTAIRE WHERE MEDICAMENT = ?");
check_meds.addBindValue(medica);
if (!check_meds.exec()) {
    qDebug() << "Failed to execute query";
    qDebug() << check_meds.lastError().text();

} else {
    int count = 0;
    if (check_meds.next()) {
            count = check_meds.value(0).toInt();
    }
    if (count > 0) {
connOpen();

        QSqlQuery query;
          query.prepare("SELECT SUM(QUANTITE) FROM INVENTAIRE WHERE MEDICAMENT= :medicament_prescrit");
          query.bindValue(":medicament_prescrit",medica);
          if (!query.exec()) {
              QMessageBox::critical(nullptr, "Erreur", "Erreur lors de l'exécution de la requête.");
          }

          int somme_quantite = 0;
          int quantite_int_de_la_prescription=quantit.toInt();
          if (query.next()) {
              somme_quantite = query.value(0).toInt();
          }

          // Vérification de la somme des quantités
          if (somme_quantite < quantite_int_de_la_prescription) {
              QString message = QString("Quantité disponible : %1 (inférieure à la quantité prescrite).").arg(somme_quantite);
              QMessageBox::information(nullptr, "Information", message);
          } else {
              QMessageBox::information(nullptr, "Confirmation", "Quantité disponible suffisante.");
              // Créer une liste d'éléments de ligne pour le modèle de tableau.
              ui->visual_ord_pour_creer_ordonnance->clear();
              ui->nom_user_affichage_pour_creer_ordonnance->show();
              ui->label_24->show();
              ui->patient_affichage_pour_creer_ordonnance->show();
              ui->label_25->show();
              ui->med_qt_ord_tab_pour_creer_ordonnance->show();
              ui->label_26->show();
              ui->date_affichage_pour_creer_ordonnance->show();
              ui->label_27->show();
              ui->creer_bouton_pour_creer_ordonnance->show();
              QList<QStandardItem*> rowItems;

              // Ajouter les éléments de ligne au modèle de tableau.
              rowItems << new QStandardItem(medica);
              rowItems << new QStandardItem(quantit);
              med_qt_ord_tab_model->appendRow(rowItems);

              // Définir le modèle de tableau pour la vue de tableau.
              ui->med_qt_ord_tab_pour_creer_ordonnance->setModel(med_qt_ord_tab_model);
          }
connClose();
    } else {

            qDebug() << "La valeur ne se trouve pas dans la table."<<count;
            QDialog *popup_check_meds = new QDialog(this);
            QPalette palette;
            palette.setColor(QPalette::Window, QColor(Qt::white));
            popup_check_meds->setPalette(palette);
            QLabel *messageLabel = new QLabel(popup_check_meds);
            messageLabel->setText("Ce médicament  est en rupture de stock");
                messageLabel->setAlignment(Qt::AlignCenter);
            // Définition du titre et de la taille de la popup
            popup_check_meds->setWindowTitle("Info");
            popup_check_meds->setFixedSize(290,100);
            QPushButton *okBtn = new QPushButton("OK", popup_check_meds);
            okBtn->setFixedSize(93,29);
            QVBoxLayout *layout = new QVBoxLayout(popup_check_meds);
            layout->addWidget(messageLabel);
            layout->addWidget(okBtn,0, Qt::AlignHCenter | Qt::AlignTop);
            connect(okBtn, &QPushButton::clicked, [=]() {
                popup_check_meds->close();
            });
            popup_check_meds->exec();

            // Affichage de la popup
            // Libération de la
            delete okBtn;
            delete layout;
            delete popup_check_meds;
    }
}
connClose();
}
//////////////////////////////////////////////création de la prescription w nsajalha f base de donnée
void MainWindow::on_creer_bouton_pour_creer_ordonnance_clicked()
{
    med_qt_ord_selectionne_tab_model->removeRows(0, med_qt_ord_selectionne_tab_model->rowCount());
    // Création d'une nouvelle fenêtre popup
QDialog *popup = new QDialog(this);
QPalette palette;
palette.setColor(QPalette::Window, QColor(Qt::white));
popup->setPalette(palette);
QLabel *messageLabel = new QLabel(popup);
messageLabel->setText("Voulez-vous vraiment créer cet ordonnance ?");
messageLabel->setAlignment(Qt::AlignCenter);
// Définition du titre et de la taille de la popup
popup->setWindowTitle("Creation Ordonnance");
popup->setFixedSize(290,100);
QPushButton *validerBtn = new QPushButton("Valider", popup);
validerBtn->setFixedSize(93,29);
QVBoxLayout *layout = new QVBoxLayout(popup);
layout->addWidget(messageLabel);
layout->addWidget(validerBtn,0, Qt::AlignHCenter | Qt::AlignTop);
connect(validerBtn, &QPushButton::clicked, [=]() {

    QString user, patient, dateord;

    user = ui->nom_user_affichage_pour_creer_ordonnance->text();
    patient = ui->patient_affichage_pour_creer_ordonnance->text();

    // Ouvre la connexion à la base de données
    if (!connOpen()) {
        qDebug() << "Failed to open the Database";
        return;
    }

    // Parcourt toutes les lignes
    for (int row = 0; row < med_qt_ord_tab_model->rowCount(); ++row) {
        // Récupère l'index de la première colonne
        QModelIndex index_med = med_qt_ord_tab_model->index(row, 0);
        QModelIndex index_qt = med_qt_ord_tab_model->index(row, 1);
        // Récupère la donnée de l'index
        QString med = index_med.data().toString();
        QString qt = index_qt.data().toString();
        dateord = ui->date_affichage_pour_creer_ordonnance->text();

        if (!connOpen()) {
            qDebug() << "Failed to open the Database";
            return;
        }
        //////////////////////////////////////////////////////////////////////////////fixer l'id de la nouvelle ordonnance
        // Exécute la requête pour obtenir la dernière valeur de ID_ORDONNANCE
        QSqlQuery query("SELECT MAX(ID_ORDONNANCE) FROM ORDONNANCE");
        if (query.next()) {
            int maxId = query.value(0).toInt();
            qDebug()<<"maxID"<<maxId;
//            if (maxId == 0) {
//                id_ord = 1;
//            } else {
                id_ord = maxId + 1;
//            }
        } else {
            qDebug() << "Failed to execute query id ordonnance";
            qDebug() << query.lastError().text();
            return;
        }
        ////////////////////////////////////////////////////////////////////////////// creation de l'ordonnance

        QSqlQuery ord;
        ord.prepare("INSERT INTO ORDONNANCE (ID_ORDONNANCE,MEDECIN,PATIENT,MEDICAMENT,QUANTITE,DATE,DISTRIBUTION) "
                    "VALUES (:id_ordonnance, :medecin, :patient, :medicament, :qte, :date,  :distribution)");

        ord.bindValue(":id_ordonnance", id_ord);
        ord.bindValue(":medecin", user);
        ord.bindValue(":patient", patient);
        ord.bindValue(":medicament", med);
        ord.bindValue(":qte", qt);
        ord.bindValue(":date", dateord);
        ord.bindValue(":distribution", 0);

        if (!ord.exec()) {
            qDebug() << "Failed to execute query creation ordonnance";
            qDebug() << ord.lastError().text();
        }

    }

    connClose();

    popup->close();

    ui->stackedWidget->setCurrentIndex(2);

});

// Affichage de la popup
popup->exec();
// Libération de la
delete validerBtn;
delete layout;
delete popup;
}
///////////////////////////////////////////////////////////////////////////////////////clique sur retour m prescription
void MainWindow::on_return_page2_from_ordonnance_clicked()
{

ui->nom_user_affichage_pour_creer_ordonnance->clear();
ui->patient_affichage_pour_creer_ordonnance->clear();;

ui->date_affichage_pour_creer_ordonnance->setText(datetext);
//    ui->nom_user->currentText().clear();
ui->patient_pour_creer_ordonnance->clear();
ui->meds_pour_creer_ordonnance->clear();
ui->qt_pour_creer_ordonnance->clear();
ui->date_pour_creer_ordonnance->clear();
model_nom_medicament_pour_creer_ordonnance->clear();
med_qt_ord_tab_model->clear();
ui->creer_bouton_pour_creer_ordonnance->hide();
ui->stackedWidget->setCurrentIndex(2);
hover_button();
}
///////////////////////////////////////////////////////////////////////////////////////clique aal bouton dispensation
void MainWindow::on_Distribution_bouton_clicked()
{
for(auto label : labels_armoir){
    label->deleteLater();
}
med_qt_ord_selectionne_tab_model->setHeaderData(0, Qt::Horizontal, "Medicament");
med_qt_ord_selectionne_tab_model->setHeaderData(1, Qt::Horizontal, "Quantité");
med_qt_ord_selectionne_tab_model->removeRows(0, med_qt_ord_selectionne_tab_model->rowCount());
med_qt_ord_selectionne_tab_model->clear();
labels_armoir.clear();
ui->visual_ord_select->setText("Veuillez choisir votre ordonnance");
ui->visual_ord_select->setAlignment(Qt::AlignCenter);
ui->num_ord->clear();
ui->medecin_ordonnance_selectionne->clear();
ui->patient_ordonnance_selectionne->clear();
ui->medecin_ordonnance_selectionne->hide();
ui->patient_ordonnance_selectionne->hide();
ui->date_ordonnance_selectionne->hide();
ui->tab_med_qt_ordonnance_selectionne->hide();
ui->date_ordonnance_selectionne->clear();
ui->label_n->hide();
ui->label_p->hide();
ui->label_med->hide();
ui->label_d->hide();
ui->Valider_3->hide();
ord_list_model->clear();
// Définit l'index de la page 4 dans le widget empilé
ui->stackedWidget->setCurrentIndex(4);
// Ouvre la connexion à la base de données
connOpen();
// Exécute la requête SQL pour récupérer les ID et les médicaments de la table ORD
QSqlQuery qryord;
if (qryord.exec("SELECT DISTINCT PATIENT,ID_ORDONNANCE FROM ORDONNANCE WHERE DISTRIBUTION = 0")) {
    // Création du modèle de données
    modif_ord_model = new QStandardItemModel(0,1, this);

    // Parcours les résultats de la requête
    while (qryord.next()) {
             patient_ord = qryord.value("PATIENT").toString();
             QString id_ordonnance =qryord.value("ID_ORDONNANCE").toString();
            // Ajoute une ligne pour chaque ordonnance non distribuée
            modif_ord_model->appendRow(new QStandardItem("Ordonnance N° "+id_ordonnance+" : "+patient_ord));
    }

    // Configure le tableau avec le modèle de données
    ui->tableau_liste_des_ordonnance->setModel(modif_ord_model);
    ui->tableau_liste_des_ordonnance->verticalHeader()->hide();
    ui->tableau_liste_des_ordonnance->horizontalHeader()->hide();

} else {
    // Affiche un message d'erreur en cas d'échec de la requête
    qDebug() << "Échec de l'exécution de la requête";
    qDebug() << qryord.lastError().text();
}
connClose();
}
/////////////////////////////////////////////////ki nselecti ordonnance taffichiheli
void MainWindow::on_tableau_liste_des_ordonnance_clicked(const QModelIndex &index)
{
med_qt_ord_selectionne_tab_model->removeRows(0, med_qt_ord_selectionne_tab_model->rowCount());
ui->visual_ord_select->clear();
ui->label_n->show();
ui->label_p->show();
ui->label_med->show();
ui->medecin_ordonnance_selectionne->show();
ui->patient_ordonnance_selectionne->show();
ui->date_ordonnance_selectionne->show();
ui->tab_med_qt_ordonnance_selectionne->show();
ui->label_d->show();

selectedItem = index.data(Qt::DisplayRole).toString();
QStringList tokens = selectedItem.split("°");
                     if (tokens.size() == 2) {
    QString numStr = tokens.at(1);
    QStringList tokens2 = numStr.split(":");
    if (tokens2.size() == 2) {
          QString numStr2 = tokens2.at(0);


    bool ok = false;
    num = numStr2.toInt(&ok);
    if (ok) {
        qDebug() << num;
qDebug()<<"Ordonnance de patient sélecté"+selectedItem;
connOpen();
QSqlQuery ordl;
ordl.prepare("SELECT ID_ORDONNANCE,MEDECIN,PATIENT,MEDICAMENT,QUANTITE,DATE FROM ORDONNANCE where ID_ORDONNANCE=:id");
ordl.bindValue(":id",  num );
// Parcourt les résultats de la requête
if (!ordl.exec()) {
              qDebug() << "Failed to execute query";
              qDebug() << ordl.lastError().text();
}

while (ordl.next()) {
               id_ordonnance_selectionne = ordl.value("ID_ORDONNANCE").toString();
              QString medecin = ordl.value("MEDECIN").toString();
              QString patient =ordl.value("PATIENT").toString();
               meds = ordl.value("MEDICAMENT").toString();
              QString quantite = ordl.value("QUANTITE").toString();
              QString date =ordl.value("DATE").toString();

              ui->num_ord->setText("Ordonnance n°"+id_ordonnance_selectionne);
              ui->label_95->setText("Ordonnance n°"+id_ordonnance_selectionne);
              ui->label_94->setText("Patient : "+patient);
              ui->medecin_ordonnance_selectionne->setText(medecin);
              ui->patient_ordonnance_selectionne->setText(patient);

                  // Insérer les éléments dans le tableau
                  QStandardItem *item_meds = new QStandardItem(meds);
                  med_qt_ord_selectionne_tab_model->setItem(0, 0, item_meds);
//                  medicament_qt_et_ouvrir_model->setItem(row, 0, item_meds);

                  QStandardItem *item_quantite = new QStandardItem(quantite);
                  med_qt_ord_selectionne_tab_model->setItem(0, 1, item_quantite);
//                  medicament_qt_et_ouvrir_model->setItem(row, 1, item_quantite);
                  row++; // Augmenter le compteur de ligne pour la prochaine itération


              ui->date_ordonnance_selectionne->setText(date);
              ui->Valider_3->show();
}

ui->tab_med_qt_ordonnance_selectionne->setModel(med_qt_ord_selectionne_tab_model);
ui->tab_med_qt_ordonnance_selectionne->setColumnHidden(2,true);
    }else {
        qDebug() << "Failed to extract integer from token:" << numStr2;
    }
    }else {
        qDebug() << "Invalid token format:" << tokens2;
    }
                     }else {
                         qDebug() << "Invalid token format:" << selectedItem;
                     }
}
////////////////////////////////////////////////////////////////////////// return l dashboard m page dispensation
void MainWindow::on_return_page2_from_distribution_clicked()
{
ui->stackedWidget->setCurrentIndex(2);
}
//////////////////////////////////////////////////////////////////////////creation de label de l'armoire dans la page de emplacement des medicament
void MainWindow::creation_label_armoire()
{
//    QLabel *labels[T];
//    QLabel *armoire[T];
//    QLabel * piogné[T];
//        QLabel *bordure;
//        // Exécution de la requête pour récupérer la dernière ligne du tableau
//        QSqlQuery query("SELECT * FROM ARMOIRE ORDER BY id DESC LIMIT 1");

//        // Vérification que la requête a réussi
//        if (query.exec()) {
//            // Récupération de la valeur de la 2ème colonne de la dernière ligne
//            if (query.next()) {
//                    QVariant  num_tir = query.value(2); // la 2ème colonne est indexée à partir de 0
//                    // Utilisation de la valeur récupérée
//                    qDebug() << "La valeur de la 2ème colonne de la dernière ligne est : " << num_tir;

//                  int num = num_tir.toInt();
//                  qDebug()<<"tiroir";
//                  T=num;
//                  int Y_T=(T*32)+230;

//                  qDebug ()<<T;
//                  qDebug ()<<Y_T;
//                  int y=208;

//                  for(int j=1;j<T+1;j++)

//            {
//              y+=32;
//                    labels[j]  = new QLabel( ui->stackedWidget->currentWidget() ); // Ajout de "this" pour parent

//                  labels[j]->setStyleSheet(

//                      "position: absolute;"
//                      "width: 513px;"
//                      "height: 280.59px;"
//                      "left: 1183.59px;"
//                      "top: 1417px;"
//                      "border: 1px solid #CFCFCF;"
//                      "background:#BFBFBF ;"
//                      "transform: rotate(90deg);"
//                      );



//                    // Positionnement du label
//                  labels[j]->setGeometry(930, y, 120, 30);
//                  labels_armoir.push_back(labels[j]);

//                 armoire[j] =new QLabel(ui->stackedWidget->currentWidget() );



//                  armoire[j]->setStyleSheet(

//                      "position: absolute;"
//                      "width: 513px;"
//                      "height: 280.59px;"
//                      "left: 1183.59px;"
//                      "top: 1417px;"
//                      "border: 1px solid #CFCFCF;"
//                      "background: #F2F2F2;"
//                      "transform: rotate(90deg);"
//                     );




//                    // Affichage du label
//                  int Y_armoire =y-5;
//                  // Positionnement du label
//                  armoire [j]->setGeometry(925,Y_armoire , 130, 55);
//                  armoire [j]->stackUnder(labels[j]);
//                  labels_armoir.push_back( armoire [j]);
//                  piogné[j] =new QLabel(ui->stackedWidget->currentWidget() );
//                                    piogné[j]->setStyleSheet(

//                                       "position: absolute;"
//                                       "width: 513px;"
//                                       "height: 280.59px;"
//                                       "left: 1183.59px;"
//                                       "top: 1417px;"
//                                       "background: #6C6C6C;"
//                                       "transform: rotate(90deg);"
//                                       );
//                  int Y_poigné=y+13;
//                   piogné[j]->setGeometry(982,Y_poigné , 16, 5);
//                  labels_armoir.push_back( piogné[j]);

//                  piogné[j]->show();

//                  labels[j]->show();
//                  armoire[j]->show();

//            }

//         bordure=new QLabel(ui->stackedWidget->currentWidget() );
//                                   bordure->setStyleSheet(

//                                       "position: absolute;"
//                                       "width: 513px;"
//                                       "height: 280.59px;"
//                                       "left: 1183.59px;"
//                                       "top: 1417px;"
//                                       "border: 1px solid #CFCFCF;"
//                                       "background: #BFBFBF;"
//                                       "transform: rotate(90deg);"
//                                       );
//         int Y_bordure=208+(T*32)+50;

//         bordure->setGeometry(925,Y_bordure , 130, 5);
//         labels_armoir.push_back( bordure);

//          bordure->show();
//         ////////////////////////////////
//        int row = med_qt_ord_tab_model->rowCount();
//        for (int i = 0; i < row; i++) {
//            // Obtient l'index de l'élément dans cette cellule
//            QModelIndex index_tiroir = med_qt_ord_tab_model->index(i, 2);

//            // Obtient les nouvelles données de l'élément
//            QVariant tiroir = index_tiroir.data();

//            // Stocke les données dans une variable
//            tiroir_pour_distribution = tiroir.toString();

//            /////////////////////////////////////////

//            int tir=tiroir_pour_distribution.toInt();

//            labels[tir]->setStyleSheet(

//                "position: absolute;"
//                "width: 513px;"
//                "height: 280.59px;"
//                "left: 1183.59px;"
//                "top: 1417px;"
//                "border: 1px solid #CFCFCF;"
//                "background: #4C8FF3;"
//                "transform: rotate(90deg);"
//                );
//            piogné[tir]->setStyleSheet(

//                                       "position: absolute;"
//                                       "width: 513px;"
//                                       "height: 280.59px;"
//                                       "left: 1183.59px;"
//                                       "top: 1417px;"
//                                       "background: #DCEBFD;"
//                                       "transform: rotate(90deg);"
//                                       );

//        }



//            } else {
//        qDebug() << "La requête a échoué : " << query.lastError().text();
//            }

//            connClose();
//        }
}
////////////////////////////////////////////////////////////////////////// clique sur valider bech nvaled ordonnance w ncreati tableau mtaa distribution
void MainWindow::on_Valider_3_clicked()
{

    for(auto label : labels_armoir){

        label->deleteLater();
    }
    labels_armoir.clear();
    for(auto label : labels){
        label->deleteLater();
    }
    labels.clear();
    ui->stackedWidget->setCurrentIndex(5);
    medicament_qt_et_ouvrir_model->setHeaderData(0, Qt::Horizontal, "Medicament");
    medicament_qt_et_ouvrir_model->setHeaderData(1, Qt::Horizontal, "Quantité");
    medicament_qt_et_ouvrir_model->setHeaderData(2, Qt::Horizontal, "Ouvrir");

    int rowCount = med_qt_ord_selectionne_tab_model->rowCount();
    for (int i = 0; i < rowCount; i++) {
    // Obtient l'index de l'élément dans cette cellule
    QModelIndex index_med = med_qt_ord_selectionne_tab_model->index(i, 0);
    QModelIndex index_qt = med_qt_ord_selectionne_tab_model->index(i, 1);

    // Obtient les nouvelles données de l'élément
    QVariant data = index_med.data();
    QVariant data_qt = index_qt.data();

    // Stocke les données dans une variable
    med_dans_tab_distribuion = data.toString();
    qt_dans_tab_distribution = data_qt.toString();
   qDebug()<<"medicament eli f ordonnance"+med_dans_tab_distribuion;
   qDebug()<<"quantite eli f ordonnance"+qt_dans_tab_distribution;

   // Ajout des boutons "Supprimer" et "Modifier" pour chaque ligne
   for (int i = 0; i < rowCount; i++) {

qDebug ()<<i;
QStandardItem *item_meds_dist = new QStandardItem(med_dans_tab_distribuion);
medicament_qt_et_ouvrir_model->setItem(i, 0, item_meds_dist);

QStandardItem *item_quantite_dist = new QStandardItem(qt_dans_tab_distribution);
medicament_qt_et_ouvrir_model->setItem(i, 1, item_quantite_dist);

ui->tab_ord_selectionne_distr->setModel(medicament_qt_et_ouvrir_model);

       QWidget *widget = new QWidget(ui->tab_ord_selectionne_distr);
       QHBoxLayout *layout = new QHBoxLayout(widget);

       QPushButton *OuvrirButton = new QPushButton("Ouvrir", widget);
       OuvrirButton->setStyleSheet("background-color: #0096DC;"
                                     "border-radius: 5px;");
       OuvrirButton->setFixedSize(172, 32);
       connect(OuvrirButton, &QPushButton::clicked, [=]() {
           on_ouvrir_clicked();
       });
           // Ajoute le bouton "Modifier" dans la première colonne de la ligne courante
           layout->addWidget(OuvrirButton);
           layout->setAlignment(Qt::AlignHCenter);
           widget->setLayout(layout);
           layout->setSizeConstraint(QLayout::SetFixedSize);
           QModelIndex index = medicament_qt_et_ouvrir_model->index(i, 2);
           ui->tab_ord_selectionne_distr->setIndexWidget(index, widget);
   }
   ui->tab_ord_selectionne_distr->setModel(medicament_qt_et_ouvrir_model);


   if (!connOpen())
   {
       qDebug() << "Impossible d'ouvrir la base de données";
           return;
   }
   ////////////////////////

   QSqlQuery tiroir_med;
   tiroir_med.prepare("SELECT TIROIR FROM INVENTAIRE WHERE MEDICAMENT LIKE :nom1 ");
   tiroir_med.bindValue(":nom1", med_dans_tab_distribuion);

   if (!tiroir_med.exec()) {
           qDebug() << "Failed to execute query tiroir";
           qDebug() << tiroir_med.lastError().text();
   }

   while (tiroir_med.next()) {
       tiroir = tiroir_med.value("TIROIR").toString();
   }
   }
    connClose();
  ///////////////////////////////////////////////////
   ///////////////////////
connOpen();

        // Exécution de la requête pour récupérer la dernière ligne du tableau
        QSqlQuery query("SELECT * FROM ARMOIRE ORDER BY id DESC LIMIT 1");

        // Vérification que la requête a réussi
        if (query.exec()) {
            // Récupération de la valeur de la 2ème colonne de la dernière ligne
            if (query.next()) {
                    QVariant  num_tir = query.value(2); // la 2ème colonne est indexée à partir de 0
                    // Utilisation de la valeur récupérée
                    qDebug() << "La valeur de la 2ème colonne de la dernière ligne est : " << num_tir;

                  int num = num_tir.toInt();
                  qDebug()<<"tiroir";
                  T=num;
                  QLabel *label[T];
                  QLabel *armoire[T];
                  QLabel * piogné[T];
                  QLabel *bordure;
                  int Y_T=(T*32)+230;

                  qDebug ()<<T;
                  qDebug ()<<Y_T;
                  int y=208;

                  for(int j=1;j<T+1;j++)

            {
              y+=32;
                    label[j]  = new QLabel( ui->stackedWidget->currentWidget() ); // Ajout de "this" pour parent

                  label[j]->setStyleSheet(


                      "border: 1px solid #CFCFCF;"
                      "background:#BFBFBF ;"
                      );



                    // Positionnement du label
                  label[j]->setGeometry(930, y, 120, 30);
                  labels_armoir.push_back(label[j]);

                 armoire[j] =new QLabel(ui->stackedWidget->currentWidget() );



                  armoire[j]->setStyleSheet(


                      "border: 1px solid #CFCFCF;"
                      "background: #F2F2F2;"
                     );

                    // Affichage du label
                  int Y_armoire =y-5;
                  // Positionnement du label
                  armoire [j]->setGeometry(925,Y_armoire , 130, 55);
                  armoire [j]->stackUnder(label[j]);
                  labels_armoir.push_back( armoire [j]);
                  piogné[j] =new QLabel(ui->stackedWidget->currentWidget() );
                                    piogné[j]->setStyleSheet(


                                       "background: #6C6C6C;"
                                       );
                  int Y_poigné=y+13;
                   piogné[j]->setGeometry(982,Y_poigné , 16, 5);
                  labels_armoir.push_back( piogné[j]);

                  piogné[j]->show();

                  label[j]->show();
                  armoire[j]->show();

            }

         bordure=new QLabel(ui->stackedWidget->currentWidget() );
                                   bordure->setStyleSheet(


                                       "border: 1px solid #CFCFCF;"
                                       "background: #BFBFBF;"
                                       );
         int Y_bordure=208+(T*32)+50;

         bordure->setGeometry(925,Y_bordure , 130, 5);
         labels_armoir.push_back( bordure);

          bordure->show();

//            /////////////////////////////////////////

            int tir=tiroir.toInt();

            label[tir]->setStyleSheet(


                "border: 1px solid #CFCFCF;"
                "background: #4C8FF3;"
                );
            piogné[tir]->setStyleSheet(


                                       "background: #DCEBFD;"
                                       );

//                }



            } else {
        qDebug() << "La requête a échoué : " << query.lastError().text();
            }

        }
        connClose();

   //////////////////////////
        //////////////////////////////////////////////////////////
        ///                 // Prépare et exécute une requête pour obtenir le tiroir correspondant à l'élément modifié
 connOpen();

        QSqlQuery creation_compartiment;
        creation_compartiment.prepare("SELECT  COMPARTIMENT FROM ARMOIRE WHERE TIROIRE=:tiroir ");
        creation_compartiment.bindValue(":tiroir",tiroir);
        if (!creation_compartiment.exec()) {
qDebug() << "Failed to execute query";
qDebug() << creation_compartiment.lastError().text();
        }

        // Parcours les résultats de la requête et met à jour l'interface utilisateur
        while (creation_compartiment.next()) {
       nbr_compartiment= creation_compartiment.value("COMPARTIMENT").toString();
        }
connClose();
///////////////////////////////////////////////

                int nb_compartiment =nbr_compartiment.toInt();
                int delay = 0; // délai initial
                QLabel *compartiment[nb_compartiment];
                int y_tiroir = 193;
//                int num_tir_stockage=tiroir.toInt();
                for (int cmp = 1; cmp < T + 1; cmp++)
                {
                y_tiroir += 32;

                QLabel *tiroir_arm = new QLabel(ui->stackedWidget->currentWidget());
                tiroir_arm->setStyleSheet(

                    "border: 1px solid #CFCFCF;"
                    "background: #BFBFBF;"
                    );
                tiroir_arm->setGeometry(1150, y_tiroir, 200, 60);
                labels.push_back(tiroir_arm);


                QPixmap image(":/images/right-arrow (1).png"); // Chargement de l'image à partir des ressources de Qt
                QLabel *imageLabel = new QLabel(ui->stackedWidget->currentWidget());
                imageLabel->setPixmap(image);
                imageLabel->setStyleSheet(
                    "background:transparent;"
                    );
                int x_image = 1060;
                int y_compartiment = y_tiroir + 5;
                imageLabel->setGeometry(x_image, y_compartiment, 70, 50);
                labels.push_back(imageLabel);

                QLabel *poigne = new QLabel(ui->stackedWidget->currentWidget());
                poigne->setStyleSheet(

                    "border: 1px solid #CFCFCF;"
                    "background: #6C6C6C;"
                    );
                int y_piogne=(y_compartiment+25)-8;
                poigne->setGeometry(1350, y_piogne, 5, 16);
                labels.push_back(poigne);

                if (cmp == tiroir.toInt())
                {

                float largeur= (200-(2*(nb_compartiment+1)))/nb_compartiment;
                qDebug ()<<"largeur";
                qDebug ()<<largeur;
                float x_compartiment = 1150+200;

                for (int i = 1; i < nb_compartiment +1; i++)
                {

                    x_compartiment -=  largeur+2;
                    qDebug ()<<"x_compartiment";
                    qDebug ()<<x_compartiment;


                    compartiment[i] = new QLabel(ui->stackedWidget->currentWidget());
                    compartiment[i]->setStyleSheet(

                        "border: 1px solid #CFCFCF;"
                        "background: #F2F2F2;"
                        );

                    compartiment[i]->setGeometry(x_compartiment, y_compartiment, largeur, 50);
                    labels.push_back(compartiment[i]);
                    compartiment[i]->show();
                }
                imageLabel->show();
                QTimer::singleShot(delay, [=]() {
                    tiroir_arm->show();
                    poigne->show();
                });
                delay += 500;

        //         Afficher chaque label avec un délai de 500 millisecondes
/////////////////
     connOpen();
                QSqlQuery query_niveau;
                query_niveau.prepare("SELECT NIVEAU FROM INVENTAIRE WHERE MEDICAMENT = :med ORDER BY NIVEAU ASC");
                query_niveau.bindValue(":med",med_dans_tab_distribuion);

                if (query_niveau.exec()) {
                    QList<QString> niveaux; // Tableau pour stocker les valeurs des champs niveau

                    while (query_niveau.next()) {
                        QString niveau = query_niveau.value(0).toString(); // Récupérer la valeur du champ niveau
                        niveaux.append(niveau); // Ajouter la valeur à la liste

                    }

                    // Vérifier si la liste a au moins trois éléments
                       if (niveaux.size() >= 1) {
                           for (int i = 0; i < qt_dans_tab_distribution.toInt() ; ++i) {
                                      int niveauInt = niveaux[i].toInt();
                                      troisPremiersInt.append(niveauInt);

                                  }
                           dernierElementInt = troisPremiersInt.last(); // Récupérer le dernier élément de la liste

                                  // Affichage des trois premiers entiers
                                  for (int niveauInt : troisPremiersInt) {
                                      qDebug() << niveauInt;
                                      compartiment[niveauInt]->setStyleSheet(

                                          "border: 1px solid #CFCFCF;"
                                          "background:#4C8FF3;"
                                          );

                                  }

                       }
                       else {
                           qDebug() << "La liste ne contient pas assez d'éléments.";
                       }
                }
                connClose();

        }}


//////////////////////////////////////////////



}
/////////////////////////////////////////////////////////////////////////////////ki nenzel aal bouton ouvrir eli f tableau

void MainWindow::ouvrir_tiroir()
{
ui->message_empreinte->clear();

if(arduino->isWritable()){


        arduino->write(tiroir.toUtf8()+"/"+nbr_compartiment.toUtf8()+"/"+ QString::number(dernierElementInt).toUtf8());
        qDebug() << "arduino ok:";
        qDebug() << tiroir.toUtf8()+"/"+nbr_compartiment.toUtf8()+"/"+ QString::number(dernierElementInt).toUtf8();
    }
}
void MainWindow::on_ouvrir_clicked()
{
    // Création d'une nouvelle fenêtre popup
                  QDialog *popup_code= new QDialog(this);
                  QPalette palette;
                  palette.setColor(QPalette::Window, QColor::fromRgb(0x26, 0x32, 0x6A));
                  popup_code->setPalette(palette);
                  // Définition du titre et de la taille de la popup
                  popup_code->setWindowTitle("Entrer code confidentiel");
                  popup_code->setFixedSize(300, 300);
                  popup_code->setStyleSheet("color:#FFFFFF;");
                  QLabel* user_label = new QLabel(user);
                  QLabel* message = new QLabel("Veuillez insérer votre code confidentiel");
                 QLineEdit* code =new QLineEdit (popup_code);
                 code->setStyleSheet("color : #000000;");
                  // Création d'un bouton pour fermer la popup
                  QPushButton *valider = new QPushButton("Valider", popup_code);
                  valider->setStyleSheet("background: #0096DC;"
                                         "color : #FFFFFF;");
                  QVBoxLayout *layout = new QVBoxLayout(popup_code);
                  layout->addWidget(user_label,0, Qt::AlignHCenter | Qt::AlignTop);
                  layout->addWidget(message,0, Qt::AlignHCenter | Qt::AlignTop);
                  layout->addWidget(code,0, Qt::AlignHCenter | Qt::AlignTop);
                  layout->addWidget(valider,0, Qt::AlignHCenter | Qt::AlignTop);

                  connect(valider, &QPushButton::clicked,[=]() {
                  QString code_text= code->text();
                      if(code_text==password_login)
                      {
                     ouvrir_tiroir();
                     for (int i = 0; i < qt_dans_tab_distribution.toInt() ; ++i) {
                                 niveauIntt = QString::number(troisPremiersInt[i]);

                     if (!connOpen())
                     {
                         qDebug() << "Impossible d'ouvrir la base de données";
                             return;
                     }
                                   QSqlQuery modif_status_ord;
                                   modif_status_ord.prepare("UPDATE  ORDONNANCE set DISTRIBUTION=:dist where ID_ORDONNANCE=:id and MEDICAMENT=:med");
                                   modif_status_ord.bindValue(":dist",1);
                                   modif_status_ord.bindValue(":id", id_ordonnance_selectionne);
                                   modif_status_ord.bindValue(":med", med_dans_tab_distribuion);
                                   if (!modif_status_ord.exec()) {
                                       qDebug() << "Failed to execute query update etat ord";
                                       qDebug() << modif_status_ord.lastError().text();
                                   }
                                   QString date_dist=datetext;
                                   QString heure_distribution=time_text;
                                   QSqlQuery distribution;
                                   distribution.prepare("INSERT INTO EVENEMENT (USER, MEDICAMENT, QUANTITE, TIROIR,COMPARTIMENT,DATE,HEURE,ID_ORDONNANCE,TYPE) "
                                                        "VALUES (:user, :medicament,:quantite,:tiroir,:compartiment, :date, :heure,:ordonnance_id,:type)");
                                   distribution.bindValue(":user", user);
                                   distribution.bindValue(":medicament",med_dans_tab_distribuion );
                                   distribution.bindValue(":tiroir", tiroir);
                                   distribution.bindValue(":compartiment", niveauIntt);
                                   distribution.bindValue(":quantite", qt_dans_tab_distribution);
                                   distribution.bindValue(":date", date_dist);
                                   distribution.bindValue(":heure", heure_distribution);
                                   distribution.bindValue(":ordonnance_id", id_ordonnance_selectionne);
                                   distribution.bindValue(":type", "Distribution");


                                   if (!distribution.exec()) {
                                       qDebug() << "Failed to execute query historique dist";
                                       qDebug() << distribution.lastError().text();
                                   }



                                   QSqlQuery inventaire;
                                       inventaire.prepare("DELETE FROM INVENTAIRE WHERE MEDICAMENT = :med AND NIVEAU = :niveau");
                                       inventaire.bindValue(":med", med_dans_tab_distribuion);
                                       inventaire.bindValue(":niveau", niveauIntt);

                                       if (!inventaire.exec()) {
                                           qDebug() << "Failed to execute query for updating inventory value";
                                           qDebug() << inventaire.lastError().text();
                                       }


                                QSqlQuery etat_niv;
                                etat_niv.prepare("update ETAT_TIROIR_NIV SET ETAT_COMPARTIMENT=:etat_compartiment where COMPARTIMENT =:compartiment and TIROIR=:tiroir");
                                 etat_niv.bindValue(":etat_compartiment", "VIDE");
                                etat_niv.bindValue(":compartiment", niveauIntt);
                                etat_niv.bindValue(":tiroir", tiroir);

                                if (!etat_niv.exec()) {
                                    qDebug() << "Failed to execute query";
                                    qDebug() << etat_niv.lastError().text();
                              }
}
                      connClose();
                      popup_code->close();

                                  QDialog *popup_fermer_tiroir= new QDialog(this);
                                  QPalette palette;
                                  palette.setColor(QPalette::Window, QColor::fromRgb(0x26, 0x32, 0x6A));
                                  popup_fermer_tiroir->setPalette(palette);
                                  // Définition du titre et de la taille de la popup
                                  popup_fermer_tiroir->setWindowTitle("Etat tiroir");
                                  popup_fermer_tiroir->setFixedSize(300, 100);
                                  popup_fermer_tiroir->setStyleSheet("color:#FFFFFF;");
                                  QLabel* message = new QLabel("Tiroir n° "+tiroir+" est ouvert");
                                  QPushButton *fermer = new QPushButton("Fermer", popup_fermer_tiroir);
                                  fermer->setStyleSheet("background: #0096DC;"
                                                         "color : #FFFFFF;");
                                  QVBoxLayout *layout2 = new QVBoxLayout(popup_fermer_tiroir);
                                  layout2->addWidget(message,0, Qt::AlignHCenter | Qt::AlignTop);
                                  layout2->addWidget(fermer,0, Qt::AlignHCenter | Qt::AlignTop);

                                  connect(fermer, &QPushButton::clicked,[=]() {
                                      on_Distribution_bouton_clicked();
                                      if(arduino->isWritable()){


                                        arduino->write(tiroir.toUtf8()+"/"+nbr_compartiment.toUtf8()+"/ 0 ");
                                          qDebug() << "arduino ok , off:";
                                          ui->message_empreinte->setText("Empreinte digitale reconnue, tiroir fermé");
                                      }
                                      popup_fermer_tiroir->close();

                                  });

                                  popup_fermer_tiroir->exec();
                                  popup_code->close();
                                  delete popup_fermer_tiroir;
                      }


 });

                  popup_code->exec();

connClose();
delete valider;
delete layout;
delete popup_code;

}
////////////////////////////////////////////////////////////////////bouton retour m emplacement medicament mtaa distribution
void MainWindow::on_return_page4_from_empl_med_clicked()
{

ui->stackedWidget->setCurrentIndex(4);
for(auto label : labels){
    label->deleteLater();
}
labels.clear();
for(auto label : labels_armoir){
    label->deleteLater();
}
labels_armoir.clear();
}
/////////////////////////////////////////////////////ki nenzel aal bouton stockage
void MainWindow::on_Stockage_bouton_clicked()
{
ui->stackedWidget->setCurrentIndex(7);
afficheur1 = new QLCDNumber(ui->stackedWidget->currentWidget());
afficheur2 = new QLCDNumber(ui->stackedWidget->currentWidget());
afficheur1->setGeometry(460,430,111,41);
afficheur2->setGeometry(460,490,111,41);
afficheur1->display("0");
afficheur2->display("0");
afficheur1->show();
afficheur2->show();
for(auto label : labels){
    label->deleteLater();
}
labels.clear();
for(auto label : labels_armoir){
    label->deleteLater();
}
labels_armoir.clear();
ui->comboBox_2->clear();
ui->comboBox_5->hide();
ui->comboBox_4->hide();
ui->stockage_indiv->hide();
ui->comboBox_2->setCurrentIndex(-1);

//////////////////////////////////////////////////////////////
    if (!connOpen())
    {
        qDebug() << "Impossible d'ouvrir la base de données";
        return;
    }

    QSqlQuery medicament_stockage;
    medicament_stockage.prepare("SELECT NOM FROM MED");

    if (!medicament_stockage.exec())
    {
        qDebug() << "Erreur lors de l'exécution de la requête : " << medicament_stockage.lastError().text();
    }
    else
    {
        while (medicament_stockage.next())
        {
             med_pour_stockage = medicament_stockage.value("NOM").toString();
            ui->comboBox_2->addItem(med_pour_stockage);
            qDebug() << "med : " << med_pour_stockage;
        }
    }

    connClose();
}
void MainWindow::on_stockage_indiv_clicked()
{
    ui->comboBox_2->clear();



//        for(auto label : labels_armoir){

//            label->deleteLater();
//        }
//        labels_armoir.clear();
//        for(auto label : labels){
//            label->deleteLater();
//        }
//        labels.clear();


}

void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{



    index = ui->comboBox_2->currentIndex();
    qDebug()<<index;
     QString quantite_a_stocker=ui->spinBox->text();

         med_a_stocker = ui->comboBox_2->itemText(index);
        qDebug()<<"med à stocker "+med_a_stocker;

        if (!connOpen())
        {
            qDebug() << "Impossible d'ouvrir la base de données";
            return;
        }

        QSqlQuery medicament_stockage;
        medicament_stockage.prepare("SELECT NOM FROM MED");

        if (!medicament_stockage.exec())
        {
            qDebug() << "Erreur lors de l'exécution de la requête : " << medicament_stockage.lastError().text();
        }
        else
        {
            while (medicament_stockage.next())
            {
                 med_pour_stockage = medicament_stockage.value("NOM").toString();
                ui->comboBox_2->addItem(med_pour_stockage);
                qDebug() << "med : " << med_pour_stockage;
            }
        }
        connClose();
        connOpen();

     QSqlQuery check_meds;
     check_meds.prepare("SELECT COUNT(*) FROM INVENTAIRE WHERE MEDICAMENT = ?");
     check_meds.addBindValue(med_a_stocker);
     if (!check_meds.exec()) {
         qDebug() << "Failed to execute query";
         qDebug() << check_meds.lastError().text();

     } else {
         int count = 0;
         if (check_meds.next()) {
                 count = check_meds.value(0).toInt();
                  qDebug()<<"numéro count";
                 qDebug()<<count;
         }

         if (count > 0) {
      connOpen();
     std::list<std::string> compartiment_vide_List;
     QSqlQuery tiroir_pour_stockage;
     tiroir_pour_stockage.prepare("SELECT DISTINCT TIROIR FROM INVENTAIRE WHERE MEDICAMENT =:meds");
     tiroir_pour_stockage.bindValue(":meds", med_a_stocker);
     if (!tiroir_pour_stockage.exec())
     {
         qDebug() << "Erreur lors de l'exécution de la requête :" << tiroir_pour_stockage.lastError().text();
     }

     while (tiroir_pour_stockage.next())
     {
          tiroir_stockage_med_existant = tiroir_pour_stockage.value("TIROIR").toString();
          qDebug()<<tiroir_stockage_med_existant;

}
     afficheur1->display(tiroir_stockage_med_existant);

connClose();
connOpen();
// Construction de la requête SQL
QString queryStr = "SELECT COMPARTIMENT FROM ETAT_TIROIR_NIV WHERE TIROIR = '" + tiroir_stockage_med_existant + "' AND ETAT_COMPARTIMENT = 'VIDE' ORDER BY COMPARTIMENT ASC";

   // Exécution de la requête SQL
   QSqlQuery query;
   if (query.exec(queryStr)) {
       QList<QString> compartimentsVides;
       while (query.next()) {
           QString compartiment = query.value(0).toString();
           compartimentsVides.append(compartiment);
       }

       // Affichage des compartiments vides récupérés
       qDebug() << "Compartiments vides du tiroir 1 :";
       for (const QString& compartiment : compartimentsVides) {
           qDebug() << compartiment;
       }
       if (!compartimentsVides.isEmpty()) {
              // Récupérez le dernier élément de la liste
               dernierElement = compartimentsVides.last();

              // Faites quelque chose avec le dernier élément récupéré
              qDebug() << "Dernier élément de la liste compartimentsVides : " << dernierElement;
              afficheur2->display(dernierElement);

          } else {
              qDebug() << "La liste compartimentsVides est vide.";
          }
   } else {
       qDebug() << "Erreur lors de l'exécution de la requête :" << query.lastError().text();
   }

   connClose();

         }

else {
             connOpen();

             std::list<std::string> tiroir_vide_List;
             QSqlQuery tiroir_pour_stockage_med_non_existant;
             tiroir_pour_stockage_med_non_existant.prepare("SELECT DISTINCT TIROIR FROM ETAT_TIROIR_NIV WHERE TIROIR NOT IN (SELECT TIROIR FROM ETAT_TIROIR_NIV WHERE ETAT_COMPARTIMENT <> 'VIDE')");

             if (tiroir_pour_stockage_med_non_existant.exec())
             {
                 while (tiroir_pour_stockage_med_non_existant.next())
                 {
                     std::string tiroir_vide_med_non_existant = tiroir_pour_stockage_med_non_existant.value("TIROIR").toString().toStdString();
                     tiroir_vide_List.push_back(tiroir_vide_med_non_existant);
                 }
             }
             // Récupération du premier élément de la liste
             if (!tiroir_vide_List.empty()) {
                  premierTiroir = tiroir_vide_List.front();
                  tiroir_stockage_med_existant = QString::fromStdString(premierTiroir);
                  qDebug ()<< "premier tiroir vide : "+tiroir_stockage_med_existant;

             }

                                             afficheur1->display(tiroir_stockage_med_existant);
connClose();
connOpen();
// Construction de la requête SQL
QString queryStr = "SELECT COMPARTIMENT FROM ETAT_TIROIR_NIV WHERE TIROIR = '" + tiroir_stockage_med_existant + "' AND ETAT_COMPARTIMENT = 'VIDE' ORDER BY COMPARTIMENT ASC";

   // Exécution de la requête SQL
   QSqlQuery query;
   if (query.exec(queryStr)) {
       QList<QString> compartimentsVides;
       while (query.next()) {
           QString compartiment = query.value(0).toString();
           compartimentsVides.append(compartiment);
       }

       // Affichage des compartiments vides récupérés
       qDebug() << "Compartiments vides du tiroir 1 :";
       for (const QString& compartiment : compartimentsVides) {
           qDebug() << compartiment;
       }
       if (!compartimentsVides.isEmpty()) {
              // Récupérez le dernier élément de la liste
               dernierElement = compartimentsVides.last();

              // Faites quelque chose avec le dernier élément récupéré
              qDebug() << "Dernier élément de la liste compartimentsVides : " << dernierElement;
              afficheur2->display(dernierElement);

          } else {
              qDebug() << "La liste compartimentsVides est vide.";
          }
   } else {
       qDebug() << "Erreur lors de l'exécution de la requête :" << query.lastError().text();
   }

   connClose();

     }
         connClose();

         }
     connClose();

               }

void MainWindow::on_return_page2_from_stockage_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->comboBox_logout->setCurrentIndex(-1);
    ui->comboBox_2->clear();
    for(auto label : labels_armoir){

        label->deleteLater();
    }
    labels_armoir.clear();
    for(auto label : labels){
        label->deleteLater();
    }
    labels.clear();
}


void MainWindow::on_Valider_stockage_clicked()
{
    if (!connOpen())
                        {
                            qDebug() << "Impossible d'ouvrir la base de données";
                                return;
                        }
    QSqlQuery creation_compartiment;
    creation_compartiment.prepare("SELECT  COMPARTIMENT FROM ARMOIRE WHERE TIROIRE=:tiroir ");
    creation_compartiment.bindValue(":tiroir",tiroir_stockage_med_existant);
    if (!creation_compartiment.exec()) {
    qDebug() << "Failed to execute query";
    qDebug() << creation_compartiment.lastError().text();
    }

    // Parcours les résultats de la requête et met à jour l'interface utilisateur
    while (creation_compartiment.next()) {
    //    num_tiroir = creation_compartiment.value("TIROIR").toString();
    nbre_compartiment= creation_compartiment.value("COMPARTIMENT").toString();
    qDebug()<<nbre_compartiment;
    }
    connClose();
    // Création d'une nouvelle fenêtre popup


    //////////////
                      QDialog *popup_code= new QDialog(this);
                      QPalette palette;
                      palette.setColor(QPalette::Window, QColor::fromRgb(0x26, 0x32, 0x6A));
                      popup_code->setPalette(palette);
                      // Définition du titre et de la taille de la popup
                      popup_code->setWindowTitle("Entrer code confidentiel");
                      popup_code->setFixedSize(300, 300);
                      popup_code->setStyleSheet("color:#FFFFFF;");
                      QLabel* user_label = new QLabel(user);
                      QLabel* message = new QLabel("Veuillez insérer votre code confidentiel");
                     QLineEdit* code =new QLineEdit (popup_code);
                     code->setStyleSheet("color : #000000;");

                      // Création d'un bouton pour fermer la popup
                      QPushButton *valider = new QPushButton("Valider", popup_code);
                      valider->setStyleSheet("background: #0096DC;"
                                             "color : #FFFFFF;");
                      QVBoxLayout *layout = new QVBoxLayout(popup_code);
                      layout->addWidget(user_label,0, Qt::AlignHCenter | Qt::AlignTop);
                      layout->addWidget(message,0, Qt::AlignHCenter | Qt::AlignTop);
                      layout->addWidget(code,0, Qt::AlignHCenter | Qt::AlignTop);
                      layout->addWidget(valider,0, Qt::AlignHCenter | Qt::AlignTop);
QString tiroir_stockage=tiroir_stockage_med_existant;
                      connect(valider, &QPushButton::clicked,[=]() {
                      QString code_text= code->text();
                          if(code_text==password_login)
                          {
                       qDebug()<<"niveau eli bech nestocki fih"+niveau_de_stockage;
    if(arduino->isWritable()){

        arduino->write(tiroir_stockage.toUtf8()+"/"+nbre_compartiment.toUtf8()+"/"+dernierElement .toUtf8());
        qDebug() << "arduino ok:";
        qDebug() << tiroir_stockage.toUtf8()+"/"+nbre_compartiment.toUtf8()+"/"+ dernierElement.toUtf8();
    }


connOpen();
                                      QString date_dist=datetext;
                                      QString heure_distribution=time_text;
                                      QSqlQuery distribution;
                                      QString quantite_a_stocker=ui->spinBox->text();
                                      distribution.prepare("INSERT INTO EVENEMENT (USER, MEDICAMENT, QUANTITE, TIROIR,COMPARTIMENT,DATE,HEURE,ID_ORDONNANCE,TYPE) "
                                                           "VALUES (:user, :medicament,:quantite,:tiroir,:compartiment, :date, :heure,:ordonnance_id,:type)");
                                      distribution.bindValue(":user", user);
                                      distribution.bindValue(":medicament",med_a_stocker);
                                      distribution.bindValue(":tiroir", tiroir_stockage_med_existant);
                                      distribution.bindValue(":compartiment", dernierElement);
                                      distribution.bindValue(":quantite",quantite_a_stocker);
                                      distribution.bindValue(":date", date_dist);
                                      distribution.bindValue(":heure", heure_distribution);
                                      distribution.bindValue(":ordonnance_id", " ");
                                      distribution.bindValue(":type", "Stockage");


                                      if (!distribution.exec()) {
                                          qDebug() << "Failed to execute query historique dist";
                                          qDebug() << distribution.lastError().text();
                                      }

   ///////////////////////

                                   QSqlQuery etat_niv;
                                   etat_niv.prepare("update ETAT_TIROIR_NIV SET ETAT_COMPARTIMENT=:etat_compartiment where COMPARTIMENT =:compartiment and TIROIR=:tiroir");
                                    etat_niv.bindValue(":etat_compartiment", "REMPLI");
                                   etat_niv.bindValue(":compartiment", dernierElement);
                                   etat_niv.bindValue(":tiroir",  tiroir_stockage_med_existant);

                                   if (!etat_niv.exec()) {
                                       qDebug() << "Failed to execute query";
                                       qDebug() << etat_niv.lastError().text();
                                 }
                                   distribution.prepare("INSERT INTO INVENTAIRE (MEDICAMENT, QUANTITE, TIROIR,NIVEAU) "
                                                        "VALUES (:medicament,:quantite,:tiroir,:niveau)");

                                   distribution.bindValue(":medicament",med_a_stocker);
                                   distribution.bindValue(":quantite",quantite_a_stocker);
                                   distribution.bindValue(":tiroir", tiroir_stockage_med_existant);
                                   distribution.bindValue(":niveau", dernierElement);



                                   if (!distribution.exec()) {
                                       qDebug() << "Failed to execute query historique dist";
                                       qDebug() << distribution.lastError().text();
                                   }
   }
                         connClose();
                          popup_code->close();

                                     QDialog *popup_fermer_tiroir= new QDialog(this);
                                     QPalette palette;
                                     palette.setColor(QPalette::Window, QColor::fromRgb(0x26, 0x32, 0x6A));
                                     popup_fermer_tiroir->setPalette(palette);
                                     // Définition du titre et de la taille de la popup
                                     popup_fermer_tiroir->setWindowTitle("Etat tiroir");
                                     popup_fermer_tiroir->setFixedSize(300, 100);
                                     popup_fermer_tiroir->setStyleSheet("color:#FFFFFF;");
                                     QLabel* message = new QLabel("Tiroir n° "+tiroir_stockage_med_existant +" est ouvert");
                                     QPushButton *fermer = new QPushButton("Fermer", popup_fermer_tiroir);
                                     fermer->setStyleSheet("background: #0096DC;"
                                                            "color : #FFFFFF;");
                                     QVBoxLayout *layout2 = new QVBoxLayout(popup_fermer_tiroir);
                                     layout2->addWidget(message,0, Qt::AlignHCenter | Qt::AlignTop);
                                     layout2->addWidget(fermer,0, Qt::AlignHCenter | Qt::AlignTop);
                                     QString tiroir_stockage_cas_fermer=tiroir_stockage_med_existant;

                                     connect(fermer, &QPushButton::clicked,[=]() {
                                         ui->comboBox_2->setCurrentIndex(-1);
                                         ui->stackedWidget->setCurrentIndex(7);

                                         if(arduino->isWritable()){


                                           arduino->write( tiroir_stockage.toUtf8()+"/"+nbre_compartiment.toUtf8()+"/ 0 ");
                                             qDebug() << "arduino ok , off:";
                                           qDebug() <<  tiroir_stockage.toUtf8()+"/"+nbre_compartiment.toUtf8()+"/ 0 ";

                                             ui->message_empreinte->setText("Empreinte digitale reconnue, tiroir fermé");
                                         }

                                         popup_fermer_tiroir->close();

                                     });
                                     popup_fermer_tiroir->exec();

                                     delete popup_fermer_tiroir;


    });
                     popup_code->exec();




   connClose();
   delete valider;
   delete layout;
   delete popup_code;

}
void MainWindow::tab_nbr_niveau_tiroir()
{
model_pour_tableau_configuration_tiroir->removeRows(0, model_pour_tableau_configuration_tiroir->rowCount());
QScrollBar *scroll_bar = ui->tab_nbr_niveau_par_tir->verticalScrollBar();
scroll_bar->setStyleSheet(
    "QScrollBar:vertical {"
    "width: 15px;"
    "margin: 15px 0 15px 0;"
    "}"
    "QScrollBar::handle:vertical {"
    "min-height: 20px;"
    "}"
    "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
    "height: 15px;"
    "width: 15px;"
    "}"
    "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
    "background: none;"
    "}"
    );
model_pour_tableau_configuration_tiroir->setHeaderData(0, Qt::Horizontal, " Armoire");
model_pour_tableau_configuration_tiroir->setHeaderData(1, Qt::Horizontal, "Tiroir");
model_pour_tableau_configuration_tiroir->setHeaderData(2, Qt::Horizontal, "Compartiment");
model_pour_tableau_configuration_tiroir->setHeaderData(3, Qt::Horizontal, "Date");

ui->tab_nbr_niveau_par_tir->verticalHeader()->hide();

if(!connOpen())
{
    qDebug() << "Failed to open the Database";
    return;
}
QSqlQuery config_tiroir;
config_tiroir.prepare("select * from  ARMOIRE ");
// Parcourt les résultats de la requête
if (!config_tiroir.exec()) {
    qDebug() << "Failed to execute query config_tiroir";
    qDebug() << config_tiroir.lastError().text();
}
while (config_tiroir.next()) {

    QString numero_armoire = config_tiroir.value("ARMOIRE").toString();
    QString nombre_des_tiroir = config_tiroir.value("TIROIRE").toString();
    QString nombre_des_compartiments = config_tiroir.value("COMPARTIMENT").toString();
    QString date_de_config =config_tiroir.value("DATE").toString();
    QList<QStandardItem*> rowItems_pour_tab_cofig_tiroir;
    rowItems_pour_tab_cofig_tiroir << new QStandardItem(numero_armoire);
    rowItems_pour_tab_cofig_tiroir << new QStandardItem(nombre_des_tiroir);
    rowItems_pour_tab_cofig_tiroir << new QStandardItem(nombre_des_compartiments);
    rowItems_pour_tab_cofig_tiroir << new QStandardItem(date_de_config);
    model_pour_tableau_configuration_tiroir->appendRow(rowItems_pour_tab_cofig_tiroir);
}
ui->tab_nbr_niveau_par_tir->setModel(model_pour_tableau_configuration_tiroir);
connClose();
}
void MainWindow::on_suivant_config_clicked()
{
ui->numero_armoire->text().clear();
ui->nbr_tiroir->text().clear();
ui->nombre_de_compartiment->text().clear();
ui->tabWidget->setCurrentIndex(1);
numero_armoire= ui->numero_armoire->text();
nbr_tir_dans_armoire= ui->nbr_tiroir->text();

if(!connOpen())
{
    qDebug() << "Failed to open the Database";
    return;
}
supprimer_tab_etat_armoire();
QSqlQuery config_niveau;
int compteur = 1;
const int maxCompteur = nbr_tir_dans_armoire.toInt();
// Supprime tous les éléments du combobox "numero_tiroir"
while (ui->numero_tiroir->count() > 0)
{
    ui->numero_tiroir->removeItem(0);
}
QSqlQuery verifier_table;
verifier_table.exec("SELECT COUNT(*) FROM ARMOIRE");
verifier_table.next();

int count = verifier_table.value(0).toInt();
if (count > 0) {
    supprimer_tab_armoire();
    while (compteur <= maxCompteur) {
            ui->numero_tiroir->addItem(QString("%1").arg(compteur));
            // Exécutez une requête pour insérer des données dans la table avec les valeurs du compteur
            config_niveau.prepare("INSERT INTO ARMOIRE (ARMOIRE, TIROIRE,DATE) VALUES (:armoire, :tiroir,:date)");

            // Bind des valeurs aux paramètres de la requête
            QString valeur1 = QString(numero_armoire).arg(compteur * 2 - 1);
            QString valeur2 = QString(QString::number(compteur)).arg(compteur * 2);
            config_niveau.bindValue(":armoire", valeur1);
            config_niveau.bindValue(":tiroir", valeur2);
            config_niveau.bindValue(":date", QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss"));

            // Exécutez la requête pour insérer la ligne actuelle
            config_niveau.exec();

            // Incrémentez le compteur pour passer à la ligne suivante
            compteur++;

    }
} if(count == 0){
    qDebug() << "La table est vide.";

    // Exécutez une boucle pour insérer les lignes
    while (compteur <= maxCompteur) {

            // Exécutez une requête pour insérer des données dans la table avec les valeurs du compteur
            config_niveau.prepare("INSERT INTO ARMOIRE (ARMOIRE, TIROIRE,DATE) VALUES (:armoire, :tiroir,:date)");

            // Bind des valeurs aux paramètres de la requête
            QString valeur1 = QString(numero_armoire).arg(compteur * 2 - 1);
            QString valeur2 = QString(QString::number(compteur)).arg(compteur * 2);
            config_niveau.bindValue(":armoire", valeur1);
            config_niveau.bindValue(":tiroir", valeur2);
            config_niveau.bindValue(":date", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

            // Exécutez la requête pour insérer la ligne actuelle
            config_niveau.exec();

            // Incrémentez le compteur pour passer à la ligne suivante
            compteur++;
    }

}

 tab_nbr_niveau_tiroir();

connClose();
}
void MainWindow::supprimer_tab_armoire()
{
QSqlQuery query;
query.exec("DELETE FROM ARMOIRE");
}

void MainWindow::supprimer_tab_etat_armoire()
{
if(!connOpen())
{
    qDebug() << "Failed to open the Database";
    return;
}
QSqlQuery verifier_table2;
verifier_table2.exec("SELECT COUNT(*) FROM ETAT_TIROIR_NIV");
verifier_table2.next();
int count2 = verifier_table2.value(0).toInt();
if (count2 > 0) {
    qDebug() << "La table contient" << count2 << "lignes.";

    // Supprimez toutes les lignes de la table
    QSqlQuery query;
    if (!query.exec("DELETE FROM ETAT_TIROIR_NIV")) {
            qDebug() << "Failed to execute query:" << query.lastError().text();
            return;
    }
}
}

void MainWindow::on_valider_config_3_clicked()
{
    int compteur = 1;
nombre_de_compartiment=ui->nombre_de_compartiment->text();
numero_tiroir=ui->numero_tiroir->currentText();
if(!connOpen())
{
    qDebug() << "Failed to open the Database";
    return;
}
QSqlQuery config_niveau;

    // Exécutez une requête pour insérer des données dans la table avec les valeurs du compteur
    config_niveau.prepare("UPDATE ARMOIRE SET COMPARTIMENT='"+nombre_de_compartiment+"' WHERE TIROIRE=:tiroir");
    config_niveau.bindValue(":tiroir", numero_tiroir);
    qDebug()<<numero_tiroir;

     qDebug()<<nombre_de_compartiment;
    if (!config_niveau.exec()) {
    qDebug() << "Failed to execute query com:" << config_niveau.lastError().text();
    return;
    }


    const int maxCompteur = nombre_de_compartiment.toInt();

    while (compteur <= maxCompteur) {

            qDebug()<<numero_tiroir;
            qDebug()<<compteur;
    QSqlQuery remplir_tab_etat;
    QString query = "INSERT INTO ETAT_TIROIR_NIV (TIROIR, COMPARTIMENT,ETAT_COMPARTIMENT,DATE_CONFIG,HEURE_CONFIG) VALUES ( :tiroir,:compartiment,:etat_compartiment,:date,:heure)";
    remplir_tab_etat.prepare(query);

    remplir_tab_etat.bindValue(":tiroir", numero_tiroir);
    remplir_tab_etat.bindValue(":compartiment", compteur);
    remplir_tab_etat.bindValue(":etat_compartiment", "VIDE");
    remplir_tab_etat.bindValue(":date", datetext);
    remplir_tab_etat.bindValue(":heure", time_text);
    if (!remplir_tab_etat.exec()) {
            qDebug() << "Failed to execute query:" << remplir_tab_etat.lastError().text();
            return;
    }

    compteur++;
    }


tab_nbr_niveau_tiroir();
connClose();
}

void MainWindow::on_return_page2_from_config_clicked()
{
ui->stackedWidget->setCurrentIndex(2);
ui->comboBox_logout->setCurrentIndex(-1);
}
void MainWindow::on_Inventaire_bouton_clicked()
{
ui->stackedWidget->setCurrentIndex(9);
inventaire_tab_model->removeRows(0,inventaire_tab_model->rowCount());

ui->tab_inventaire->verticalHeader()->hide();
    inventaire_tab_model->setHeaderData(0, Qt::Horizontal, "Médicament");
    inventaire_tab_model->setHeaderData(1, Qt::Horizontal, "Quantité");
    inventaire_tab_model->setHeaderData(2, Qt::Horizontal, "Tiroir");
    inventaire_tab_model->setHeaderData(3, Qt::Horizontal, "Niveau");
    QScrollBar *scroll_bar = ui->tab_inventaire->verticalScrollBar();
    scroll_bar->setStyleSheet(
        "QScrollBar:vertical {"
        "width: 15px;"
        "margin: 15px 0 15px 0;"
        "}"
        "QScrollBar::handle:vertical {"
        "min-height: 20px;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "height: 15px;"
        "width: 15px;"
        "}"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
        "background: none;"
        "}"
        );
    if(!connOpen())
    {
    qDebug() << "Failed to open the Database";
    return;
    }
    QSqlQuery inventaire_affichage;
    inventaire_affichage.prepare("select * from  INVENTAIRE ");

    // Parcourt les résultats de la requête
    if (!inventaire_affichage.exec()) {
    qDebug() << "Failed to execute query";
    qDebug() << inventaire_affichage.lastError().text();
    }
    while (inventaire_affichage.next()) {
    QList<QStandardItem*> rowItems;

    QString medicament_inventaire = inventaire_affichage.value("MEDICAMENT").toString();
    QString qt_medicament_inventaire = inventaire_affichage.value("QUANTITE").toString();
    QString tiroir_medicament_inventaire = inventaire_affichage.value("TIROIR").toString();
    QString niveau_tiroir_medicament_inventaire = inventaire_affichage.value("NIVEAU").toString();

    rowItems << new QStandardItem(medicament_inventaire);
    rowItems << new QStandardItem(qt_medicament_inventaire);
    rowItems << new QStandardItem(tiroir_medicament_inventaire);
    rowItems << new QStandardItem(niveau_tiroir_medicament_inventaire);
    inventaire_tab_model->appendRow(rowItems);

    }
    ui->tab_inventaire->setModel(inventaire_tab_model);
    connClose();
     QPieSeries *series = new QPieSeries;
     QMap<QString, int> medicamentQuantites;
    for (int i = 0; i < inventaire_tab_model->rowCount(); i++) {

            QString name = inventaire_tab_model->index(i, 0).data().toString();
            int qty = inventaire_tab_model->index(i, 1).data().toInt();
            if (medicamentQuantites.contains(name)) {
            medicamentQuantites[name] += qty;
            } else {
            medicamentQuantites[name] = qty;
            }
            qDebug() << "Name: " << name << ", quantity: " << qty;


    }
    // Afficher la somme des quantités pour chaque médicament
    QMapIterator<QString, int> iterator(medicamentQuantites);
    while (iterator.hasNext()) {
            iterator.next();
            QString name = iterator.key();
            int qty = iterator.value();
            qDebug() << "Médicament: " << name << ", somme des quantités: " << qty;
                    QPieSlice *slice = series->append(name, qty);
            slice->setLabel(QString("%1 (%2)").arg(name).arg(qty));
    }
    // Add the "Others" slice

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Médicaments");
    chart->setTitleFont(QFont("Montserrat", 20));
    QColor myColor("#26326A");
    QBrush titleBrush(myColor); // création d'un objet QBrush avec la couleur rouge
    chart->setTitleBrush(titleBrush); // application de la couleur au titre
    chart->legend()->setFont(QFont("Arial", 10));
    chart->legend()->setAlignment(Qt::AlignRight);
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);// enable OpenGL rendering for the legend
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setParent(ui->horizontalFrame);
    chartView->setGeometry(0,0,671,388);
    chartView->setAlignment(Qt::AlignHCenter);
    chartView->show();
    connect(ui->tab_inventaire->selectionModel(), &QItemSelectionModel::selectionChanged, [=](const QItemSelection &selected, const QItemSelection &deselected) {
        if (selected.indexes().isEmpty()) {
            return;}
        else {
            QModelIndex index = selected.indexes().at(0);

            QString selected_name = ui->tab_inventaire->model()->index(selected.indexes().at(0).row(), 0).data().toString();
            QString selected_quantity = ui->tab_inventaire->model()->index(selected.indexes().at(0).row(), 1).data().toString();
            bool found = false;
            for (int i = 0; i < series->count(); i++) {
                QPieSlice *slice = series->slices().at(i);
                if (slice->label().startsWith(selected_name)) {
                    found = true;
                    slice->setExploded(true);
                    slice->setLabel(QString("%1 (%2)").arg(selected_name).arg(selected_quantity));
                } else {
                    slice->setExploded(false);
                }
            }

        }
    });
    for (int i = 0; i < series->count(); i++) {
    QPieSlice *slice = series->slices().at(i);
    connect(slice, &QPieSlice::clicked, [=]() {
        // First set all slices to not exploded
        for (int j = 0; j < series->count(); j++) {
            series->slices().at(j)->setExploded(false);
        }
        // Then explode the selected slice
        slice->setExploded(true);
        qDebug() << "Slice clicked: " << slice->label();
        // Select the corresponding row in the connected table
        for (int j = 0; j < inventaire_tab_model->rowCount(); j++) {
            QString name = inventaire_tab_model->index(j, 0).data().toString();
            if (slice->label().startsWith(name)) {
                if (name == "Others") {
                    // If the slice is "Others", just explode it without selecting a row
                } else {
                    // Otherwise, select the corresponding row in the table
                    ui->tab_inventaire->selectRow(j);
                }
                break;
            }
        }
    });
    }

}
void MainWindow::on_return_page2_from_inventaire_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->comboBox_logout->setCurrentIndex(-1);

}
////////////////////////////////////////////////////////****************consulter historique
void MainWindow::on_historique_bouton_clicked()
{
    historique_tab->removeRows(0, historique_tab->rowCount());

    QScrollBar *scroll_bar = ui->tab_historique->verticalScrollBar();
    scroll_bar->setStyleSheet(
        "QScrollBar:vertical {"
        "width: 15px;"
        "margin: 15px 0 15px 0;"
        "}"
        "QScrollBar::handle:vertical {"
        "min-height: 20px;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "height: 15px;"
        "width: 15px;"
        "}"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
        "background: none;"
        "}"
        );
  ui->stackedWidget->setCurrentIndex(10);
  ui->utilisateur->clear();
  historique_tab->removeRows(0, historique_tab->rowCount());
  historique_tab->setHeaderData(0, Qt::Horizontal, "Utilisateur");
  historique_tab->setHeaderData(1, Qt::Horizontal, "Médicament");
      historique_tab->setHeaderData(2, Qt::Horizontal, "Quantité");
      historique_tab->setHeaderData(3, Qt::Horizontal, "Tiroir");
  historique_tab->setHeaderData(4, Qt::Horizontal, "Compartiment");
  historique_tab->setHeaderData(5, Qt::Horizontal, "Date");
  historique_tab->setHeaderData(6, Qt::Horizontal, "Heure");
  historique_tab->setHeaderData(7, Qt::Horizontal, "Type");
}
//////////////////////////////////////////////////////////////////////********** historique filtré par les evenements*****************
void MainWindow::on_activite_currentIndexChanged(int index)
{
  historique_tab->removeRows(0, historique_tab->rowCount());

   type_activite = ui->activite->currentText();
  qDebug() << type_activite;

  ///////////////////////////////
  if(!connOpen())
  {
    qDebug() << "Failed to open the Database";
    return;
  }

  //////////////////////////////////////***************************historique stockage
  QSqlQuery activite;
  if(type_activite=="Stockage")
  {
    activite.prepare("select USER,MEDICAMENT,QUANTITE,COMPARTIMENT,TIROIR,DATE,HEURE,TYPE from  EVENEMENT where TYPE=:type ");
    activite.bindValue(":type", "Stockage");


    // Parcourt les résultats de la requête
    if (!activite.exec()) {
            qDebug() << "Failed to execute query";
            qDebug() << activite.lastError().text();
    }
    while (activite.next()) {
            QList<QStandardItem*> rowItems;
            QString username_hist = activite.value("USER").toString();
            QString medicament_hist_user = activite.value("MEDICAMENT").toString();
            QString qt_medicament_hist_user = activite.value("QUANTITE").toString();
            QString tir_medicament_hist_user = activite.value("TIROIR").toString();
            QString compartiment_tir_medicament_hist_user = activite.value("COMPARTIMENT").toString();
            QString date_medicament_hist_user = activite.value("DATE").toString();
            QString heure_medicament_hist_user = activite.value("HEURE").toString();
            QString type_medicament_hist_user = activite.value("TYPE").toString();

            // Ajouter les éléments de ligne au modèle de tableau.
            rowItems << new QStandardItem(username_hist);
            rowItems << new QStandardItem(medicament_hist_user);
            rowItems << new QStandardItem(qt_medicament_hist_user);
            rowItems << new QStandardItem(tir_medicament_hist_user);
            rowItems << new QStandardItem(date_medicament_hist_user);
            rowItems << new QStandardItem(heure_medicament_hist_user);
            rowItems << new QStandardItem(type_medicament_hist_user);

            historique_tab->appendRow(rowItems);

    }
  }
  ///////////////////////////////////////************historique distribution
  if(type_activite=="Distribution")
  {
    activite.prepare("select USER,MEDICAMENT,QUANTITE,COMPARTIMENT,TIROIR,DATE,HEURE,TYPE from  EVENEMENT where TYPE=:type ");
    activite.bindValue(":type", "Distribution");


    // Parcourt les résultats de la requête
    if (!activite.exec()) {
            qDebug() << "Failed to execute query";
            qDebug() << activite.lastError().text();
    }
    while (activite.next()) {
            QList<QStandardItem*> rowItems;
            QString username_hist = activite.value("USER").toString();
            QString medicament_hist_user = activite.value("MEDICAMENT").toString();
            QString qt_medicament_hist_user = activite.value("QUANTITE").toString();
            QString tir_medicament_hist_user = activite.value("TIROIR").toString();
            QString compartiment_tir_medicament_hist_user = activite.value("COMPARTIMENT").toString();
            QString date_medicament_hist_user = activite.value("DATE").toString();
            QString heure_medicament_hist_user = activite.value("HEURE").toString();
            QString type_medicament_hist_user = activite.value("TYPE").toString();

            // Ajouter les éléments de ligne au modèle de tableau.
            rowItems << new QStandardItem(username_hist);
            rowItems << new QStandardItem(medicament_hist_user);
            rowItems << new QStandardItem(qt_medicament_hist_user);
            rowItems << new QStandardItem(tir_medicament_hist_user);
            rowItems << new QStandardItem(compartiment_tir_medicament_hist_user);
            rowItems << new QStandardItem(date_medicament_hist_user);
            rowItems << new QStandardItem(heure_medicament_hist_user);
            rowItems << new QStandardItem(type_medicament_hist_user);

            historique_tab->appendRow(rowItems);

    }
  }

  ui->tab_historique->setModel(historique_tab);
  connClose();

}
///////////////////////////////************************ historique filtré par utilisateur******************

void MainWindow::on_utilisateur_returnPressed()
{
  historique_tab->removeRows(0, historique_tab->rowCount());

  QString user_hist_name=ui->utilisateur->text();
  qDebug()<<user_hist_name;

  if(!connOpen())
  {
    qDebug() << "Failed to open the Database";
    return;
  }


  QSqlQuery fonct_of_user;

  fonct_of_user.prepare("select ROLE from  USER where USERNAME=:name");
  fonct_of_user.bindValue(":name", user_hist_name);
  if (!fonct_of_user.exec()) {
    qDebug() << "Failed to execute query";
    qDebug() << fonct_of_user.lastError().text();
  }
  while (fonct_of_user.next()) {
    fonct_user = fonct_of_user.value("ROLE").toString();
    qDebug ()<<fonct_user;
  }
    QSqlQuery hist_of_user;
  if(type_activite=="Stockage")
  {
    hist_of_user.prepare("select USER,MEDICAMENT,QUANTITE,COMPARTIMENT,TIROIR,DATE,HEURE,TYPE from  EVENEMENT where USER LIKE :name and TYPE =:type");
    hist_of_user.bindValue(":name", user_hist_name);
    hist_of_user.bindValue(":type", type_activite);


    // Parcourt les résultats de la requête
    if (!hist_of_user.exec()) {
            qDebug() << "Failed to execute query";
            qDebug() << hist_of_user.lastError().text();
    }
    while (hist_of_user.next()) {
            QList<QStandardItem*> rowItems;
            QString username_hist = hist_of_user.value("USER").toString();
            QString medicament_hist_user = hist_of_user.value("MEDICAMENT").toString();
            QString qt_medicament_hist_user = hist_of_user.value("QUANTITE").toString();
            QString tir_medicament_hist_user = hist_of_user.value("TIROIR").toString();
            QString compartiment_tir_medicament_hist_user = hist_of_user.value("COMPARTIMENT").toString();
            QString date_medicament_hist_user = hist_of_user.value("DATE").toString();
            QString heure_medicament_hist_user = hist_of_user.value("HEURE").toString();
            QString type_medicament_hist_user = hist_of_user.value("TYPE").toString();

            // Ajouter les éléments de ligne au modèle de tableau.
            rowItems << new QStandardItem(username_hist);
            rowItems << new QStandardItem(medicament_hist_user);
            rowItems << new QStandardItem(qt_medicament_hist_user);
            rowItems << new QStandardItem(tir_medicament_hist_user);
            rowItems << new QStandardItem(compartiment_tir_medicament_hist_user);
            rowItems << new QStandardItem(date_medicament_hist_user);
            rowItems << new QStandardItem(heure_medicament_hist_user);
            rowItems << new QStandardItem(type_medicament_hist_user);

            historique_tab->appendRow(rowItems);

    }
  }
  else {
  if(type_activite=="Distribution")
  {
    hist_of_user.prepare("select USER,MEDICAMENT,QUANTITE,COMPARTIMENT,TIROIR,DATE,HEURE,TYPE from  EVENEMENT where USER LIKE :name and TYPE =:type");
    hist_of_user.bindValue(":name", user_hist_name);
    hist_of_user.bindValue(":type", type_activite);


    // Parcourt les résultats de la requête
    if (!hist_of_user.exec()) {
            qDebug() << "Failed to execute query";
            qDebug() << hist_of_user.lastError().text();
    }
    while (hist_of_user.next()) {
            QList<QStandardItem*> rowItems;
            QString username_hist = hist_of_user.value("USER").toString();
            QString medicament_hist_user = hist_of_user.value("MEDICAMENT").toString();
            QString qt_medicament_hist_user = hist_of_user.value("QUANTITE").toString();
            QString tir_medicament_hist_user = hist_of_user.value("TIROIR").toString();
            QString compartiment_tir_medicament_hist_user = hist_of_user.value("COMPARTIMENT").toString();
            QString date_medicament_hist_user = hist_of_user.value("DATE").toString();
            QString heure_medicament_hist_user = hist_of_user.value("HEURE").toString();
            QString type_medicament_hist_user = hist_of_user.value("TYPE").toString();

            // Ajouter les éléments de ligne au modèle de tableau.
            rowItems << new QStandardItem(username_hist);
            rowItems << new QStandardItem(medicament_hist_user);
            rowItems << new QStandardItem(qt_medicament_hist_user);
            rowItems << new QStandardItem(tir_medicament_hist_user);
            rowItems << new QStandardItem(compartiment_tir_medicament_hist_user);
            rowItems << new QStandardItem(date_medicament_hist_user);
            rowItems << new QStandardItem(heure_medicament_hist_user);
            rowItems << new QStandardItem(type_medicament_hist_user);

            historique_tab->appendRow(rowItems);

    }

  }
  else {
    hist_of_user.prepare("select USER,MEDICAMENT,QUANTITE,COMPARTIMENT,TIROIR,DATE,HEURE,TYPE from  EVENEMENT where USER LIKE :name");
    hist_of_user.bindValue(":name", user_hist_name);
    hist_of_user.bindValue(":type", type_activite);


    // Parcourt les résultats de la requête
    if (!hist_of_user.exec()) {
            qDebug() << "Failed to execute query";
            qDebug() << hist_of_user.lastError().text();
    }
    while (hist_of_user.next()) {
            QList<QStandardItem*> rowItems;
            QString username_hist = hist_of_user.value("USER").toString();
            QString medicament_hist_user = hist_of_user.value("MEDICAMENT").toString();
            QString qt_medicament_hist_user = hist_of_user.value("QUANTITE").toString();
            QString tir_medicament_hist_user = hist_of_user.value("TIROIR").toString();
            QString compartiment_tir_medicament_hist_user = hist_of_user.value("COMPARTIMENT").toString();
            QString date_medicament_hist_user = hist_of_user.value("DATE").toString();
            QString heure_medicament_hist_user = hist_of_user.value("HEURE").toString();
            QString type_medicament_hist_user = hist_of_user.value("TYPE").toString();

            // Ajouter les éléments de ligne au modèle de tableau.
            rowItems << new QStandardItem(username_hist);
            rowItems << new QStandardItem(medicament_hist_user);
            rowItems << new QStandardItem(qt_medicament_hist_user);
            rowItems << new QStandardItem(tir_medicament_hist_user);
            rowItems << new QStandardItem(compartiment_tir_medicament_hist_user);
            rowItems << new QStandardItem(date_medicament_hist_user);
            rowItems << new QStandardItem(heure_medicament_hist_user);
            rowItems << new QStandardItem(type_medicament_hist_user);

            historique_tab->appendRow(rowItems);

  }
  }
  }
  ui->tab_historique->setModel(historique_tab);
  connClose();
}
///////////////////////////////************************ historique filtré par date******************

void MainWindow::on_date_hist_dateChanged(const QDate &date)
{
  historique_tab->removeRows(0, historique_tab->rowCount());

  QString date_activite_hist = ui->date_hist->text();
  qDebug() << date_activite_hist;


  if(!connOpen())
  {
            qDebug() << "Failed to open the Database";
            return;
  }

  QSqlQuery date_activite;


  date_activite.prepare("select USER,MEDICAMENT,QUANTITE,TIROIR,COMPARTIMENT,DATE,HEURE,TYPE from  EVENEMENT where DATE=:date ");
  date_activite.bindValue(":date", date_activite_hist);


  // Parcourt les résultats de la requête
  if (!date_activite.exec()) {
            qDebug() << "Failed to execute query";
            qDebug() << date_activite.lastError().text();
  }
  while (date_activite.next()) {
            QList<QStandardItem*> rowItems;
            QString username_hist = date_activite.value("USER").toString();
            QString medicament_hist_user = date_activite.value("MEDICAMENT").toString();
            QString qt_medicament_hist_user = date_activite.value("QUANTITE").toString();
            QString tir_medicament_hist_user = date_activite.value("TIROIR").toString();
            QString compartiment_tir_medicament_hist_user = date_activite.value("COMPARTIMENT").toString();
            QString date_medicament_hist_user = date_activite.value("DATE").toString();
            QString heure_medicament_hist_user = date_activite.value("HEURE").toString();
            QString type_medicament_hist_user = date_activite.value("TYPE").toString();

            // Ajouter les éléments de ligne au modèle de tableau.
            rowItems << new QStandardItem(username_hist);
            rowItems << new QStandardItem(medicament_hist_user);
            rowItems << new QStandardItem(qt_medicament_hist_user);
            rowItems << new QStandardItem(tir_medicament_hist_user);
            rowItems << new QStandardItem(compartiment_tir_medicament_hist_user);
            rowItems << new QStandardItem(date_medicament_hist_user);
            rowItems << new QStandardItem(heure_medicament_hist_user);
            rowItems << new QStandardItem(type_medicament_hist_user);

            historique_tab->appendRow(rowItems);

  }


  ui->tab_historique->setModel(historique_tab);
  connClose();
}



void MainWindow::on_return_page2_from_historique_clicked()
{
ui->stackedWidget->setCurrentIndex(2);
ui->comboBox_logout->setCurrentIndex(-1);


}

void MainWindow::on_bouton_liste_des_utilisateurs_clicked()
{
    ui->stackedWidget_3->setCurrentIndex(0);
    creation_tableau_liste_des_utilisateurs();
}


void MainWindow::on_bouton_ajouter_utilisateur_clicked()
{
ui->stackedWidget_3->setCurrentIndex(1);
ui->medecin->hide();
ui->nom_utilisateur->clear();
ui->mot_de_passe->clear();
ui->role->setCurrentIndex(-1);
ui->medecin->clear();
}

void MainWindow::creation_tableau_liste_des_utilisateurs()

{
    model_tableau_utilisateur->removeRows(0,model_tableau_utilisateur->rowCount());


    ui->tab_liste_des_utilisateurs->verticalHeader()->hide();

    QPalette palette_tab_utilisateur;
    palette_tab_utilisateur.setColor(QPalette::Window, QColor(Qt::white));
    QString styleSheet = "QTableView { gridline-color: white; background : #F5F5F5 }";
    ui->tab_liste_des_utilisateurs->setStyleSheet(styleSheet);
    model_tableau_utilisateur->setHeaderData(0, Qt::Horizontal, "Nom d'utilisateur");
    model_tableau_utilisateur->setHeaderData(1, Qt::Horizontal, "Mot de passe");
    model_tableau_utilisateur->setHeaderData(2, Qt::Horizontal, "Fonction");
    model_tableau_utilisateur->setHeaderData(3, Qt::Horizontal, "Médecin");
    model_tableau_utilisateur->setHeaderData(4, Qt::Horizontal, "Supprimer");
    model_tableau_utilisateur->setHeaderData(5, Qt::Horizontal, "Modifier");




    if (!connOpen()) {
        qDebug() << "Impossible d'ouvrir la base de données";
        return;
    }

    QSqlQuery liste_des_utilisateurs;
    liste_des_utilisateurs.prepare("SELECT * FROM USER");
    if (!liste_des_utilisateurs.exec()) {
        qDebug() << "Failed to execute query liste des utilisateurs";
        qDebug() << liste_des_utilisateurs.lastError().text();
        return;
    }

    // Parcours les résultats de la requête et met à jour l'interface utilisateur
    while (liste_des_utilisateurs.next()) {
        QList<QStandardItem*> rowItems;
        QString nom_utilisateur_pour_liste = liste_des_utilisateurs.value("USERNAME").toString();
        QString mot_de_passe = liste_des_utilisateurs.value("PASSWORD").toString();
        QString role_utilisateur = liste_des_utilisateurs.value("ROLE").toString();
        QString medecin_appartenant = liste_des_utilisateurs.value("MEDECIN").toString();

        rowItems << new QStandardItem(nom_utilisateur_pour_liste);
        rowItems << new QStandardItem(mot_de_passe);
        rowItems << new QStandardItem(role_utilisateur);
        rowItems << new QStandardItem(medecin_appartenant);
        model_tableau_utilisateur->appendRow(rowItems);
    }

    int rowCount = model_tableau_utilisateur->rowCount();
    for (int i = 0; i < rowCount; i++) {
        QWidget *widget = new QWidget(ui->tab_liste_des_utilisateurs);
        QHBoxLayout *layout = new QHBoxLayout(widget);
        QPushButton *supprimer = new QPushButton("Supprimer", widget); // Modification : renommage du bouton
        supprimer->setStyleSheet("background-color: red ;"
                                  "border-radius: 7px;"
                                 "color: white;");
        supprimer->setFixedSize(80,20);
        connect(supprimer, &QPushButton::clicked, [=]() {
            QModelIndex index = model_tableau_utilisateur->index(i, 4); // Modification : index de la colonne supprimer
            username_for_delete = model_tableau_utilisateur->index(index.row(), 0).data().toString(); // Modification : index de la colonne USERNAME
            qDebug() << username_for_delete;
            QDialog *popup_delete_user = new QDialog(this);
            QPalette palette;
            palette.setColor(QPalette::Window, QColor(Qt::white));
            popup_delete_user->setPalette(palette);

             QLabel *messageLabel = new QLabel(popup_delete_user);
             messageLabel->setText("Voulez-vous vraiment supprimer cet utilisateur ?");
             messageLabel->setAlignment(Qt::AlignCenter);
            // Définition du titre et de la taille de la popup
            popup_delete_user->setWindowTitle("Supprimer");
            popup_delete_user->setFixedSize(400, 300);
            QPushButton *validerBtn = new QPushButton("Valider", popup_delete_user);
             QVBoxLayout *layout = new QVBoxLayout(popup_delete_user);
             layout->addWidget(messageLabel);
             layout->addWidget(validerBtn);

            connect(validerBtn, &QPushButton::clicked, [=]() {
                if(!connOpen())
                {
                    qDebug() << "Failed to open the Database";
                    return;
                }
             QSqlQuery remove_user;
             remove_user.prepare("DELETE  FROM USER where USERNAME='"+username_for_delete+"'");
             if (!remove_user.exec()) {
                 qDebug() << "Failed to execute query";
                 qDebug() << remove_user.lastError().text();
             }
             model_tableau_utilisateur->removeRows(0,model_tableau_utilisateur->rowCount());

         creation_tableau_liste_des_utilisateurs();
             popup_delete_user->close();

             });
        connClose();
         // Affichage de la popup
         popup_delete_user->exec();

         // Libération de la
         delete validerBtn;
         delete layout;
         delete popup_delete_user;

        });
        layout->addWidget(supprimer);
        layout->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter|Qt::AlignTop);
        widget->setLayout(layout);
        widget->setStyleSheet("background-color:#F5F5F5;");
        QModelIndex index = model_tableau_utilisateur->index(i, 4); // Modification : index de la colonne supprimer
        ui->tab_liste_des_utilisateurs->setIndexWidget(index, widget); // Modification : utilisation de tab_liste_des_utilisateurs au lieu de tab_ord_selectionne_distr
        ////////////////////
        QWidget *widget2 = new QWidget(ui->tab_liste_des_utilisateurs);
        QHBoxLayout *layout2 = new QHBoxLayout(widget2);
        QPushButton *modifier = new QPushButton("Modifier", widget2); // Modification : renommage du bouton
        modifier->setStyleSheet("background-color: #ffbe3d ;"
                                  "border-radius: 7px;"
                                 "color: white;");
        modifier->setFixedSize(80,20);
        connect(modifier, &QPushButton::clicked, [=]() {
            QModelIndex index = model_tableau_utilisateur->index(i, 4); // Modification : index de la colonne supprimer
            username_for_delete = model_tableau_utilisateur->index(index.row(), 0).data().toString(); // Modification :
qDebug()<<username_for_delete;
            on_modify_clicked();
            model_tableau_utilisateur->removeRows(0,model_tableau_utilisateur->rowCount());
            creation_tableau_liste_des_utilisateurs();
        });
        layout2->addWidget(modifier);
        layout2->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter|Qt::AlignTop);
        widget2->setLayout(layout2);
        widget2->setStyleSheet("background-color:#F5F5F5;");
        QModelIndex index2 = model_tableau_utilisateur->index(i, 5); // Modification : index de la colonne supprimer
        ui->tab_liste_des_utilisateurs->setIndexWidget(index2, widget2);

    }

    ui->tab_liste_des_utilisateurs->setModel(model_tableau_utilisateur);
    connClose();

    ////////////////////////////////////





}
void MainWindow::on_Utilisateurs_bouton_clicked()

{

    ui->stackedWidget->setCurrentIndex(11);
    ui->stackedWidget_3->setCurrentIndex(0);
    on_bouton_liste_des_utilisateurs_clicked();
    creation_tableau_liste_des_utilisateurs();
}


void MainWindow::on_return_page2_from_historique_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);

}
void MainWindow::on_confirmer_nouvel_utilisateur_clicked()
{

    model_tableau_utilisateur->removeRows(0,model_tableau_utilisateur->rowCount());
     nom_utilisateur_line_edit = ui->nom_utilisateur->text();
     mot_de_passe_line_edit = ui->mot_de_passe->text();
     fonction_line_edit = ui->role->currentText();
     medecin_line_edit = ui->medecin->text();
     if(!nom_utilisateur_line_edit.isEmpty()&& !mot_de_passe_line_edit.isEmpty())
     {
    if (!connOpen())
    {
        qDebug() << "Impossible d'ouvrir la base de données";
        return;
    }

    QSqlQuery ajout_nouvel_utilisateur;
    ajout_nouvel_utilisateur.prepare("INSERT INTO USER (USERNAME,PASSWORD,ROLE,MEDECIN) VALUES ( :username,:password,:role,:medecin)");
    ajout_nouvel_utilisateur.bindValue(":username", nom_utilisateur_line_edit);
    ajout_nouvel_utilisateur.bindValue(":password", mot_de_passe_line_edit);
    ajout_nouvel_utilisateur.bindValue(":role", fonction_line_edit);
    ajout_nouvel_utilisateur.bindValue(":medecin", medecin_line_edit);

    if (!ajout_nouvel_utilisateur.exec()) {
    qDebug() << "Failed to execute query liste des utilisateurs";
    qDebug() << ajout_nouvel_utilisateur.lastError().text();
    }
    connClose();
    creation_tableau_liste_des_utilisateurs();
    ////////////////////////////////////////////////////////////////////////////////
    if (!connOpen())
    {
        qDebug() << "Impossible d'ouvrir la base de données";
        return;
    }
    QSqlQuery get_id;
    get_id.prepare("select ID from USER where USERNAME=:nom");
    get_id.bindValue(":nom", nom_utilisateur_line_edit);
    if (!get_id.exec()) {
        qDebug() << "Failed to execute query";
        qDebug() << get_id.lastError().text();
    }

    while (get_id.next()) {
          id_new_user=get_id.value("ID").toString();
         qDebug()<<id_new_user;
    }
    connClose();

    //////////////////////////////////////////////////////////////////////////////////
    // Création d'une nouvelle fenêtre popup
                  QDialog *popup_finger = new QDialog(this);
                  QPalette palette;
                  palette.setColor(QPalette::Window, QColor::fromRgb(0x26, 0x32, 0x6A));
                  popup_finger->setPalette(palette);
                  // Définition du titre et de la taille de la popup
                  popup_finger->setWindowTitle("Ajout empreinte");
                  popup_finger->setFixedSize(500, 500);

                  QLabel* id = new QLabel("ID :"+ id_new_user);
                  QLabel* message = new QLabel("Veuillez insérer votre empreinte");
                  message->setStyleSheet("color: #FFFFFF;");
                  id->setStyleSheet("color: #FFFFFF;");
                  QPixmap image(":/images/fingerprint.png");  // Chargement de l'image à partir des ressources de Qt
                  QLabel *imageLabel = new QLabel;
                  imageLabel->setPixmap(image);
                  // Création d'un bouton pour fermer la popup
                  QPushButton *enregistrer = new QPushButton("Enregistrer", popup_finger);
                  enregistrer->setStyleSheet("background: #0096DC;"
                                             "color: #FFFFFF;");
                   QPushButton *annuler = new QPushButton("Annuler", popup_finger);
                   annuler->setStyleSheet("background: #0096DC;"
                                          "color: #FFFFFF;");
                  QVBoxLayout *layout = new QVBoxLayout(popup_finger);
                  layout->addWidget(id,0, Qt::AlignHCenter | Qt::AlignTop);
                  layout->addWidget(message,0, Qt::AlignHCenter | Qt::AlignTop);
                  layout->addWidget(imageLabel,0, Qt::AlignHCenter | Qt::AlignTop);
                  layout->addWidget(enregistrer,0, Qt::AlignHCenter | Qt::AlignTop);
                  layout->addWidget(annuler,0, Qt::AlignHCenter | Qt::AlignTop);
                  if(arduino->isWritable()){
                          arduino->write(id_new_user.toUtf8());
                          qDebug() << "arduino ok:";
                          qDebug() << id_new_user.toUtf8();
                      }

                  connect(enregistrer, &QPushButton::clicked,[=]() {
                          ui->stackedWidget_3->setCurrentIndex(0);
                            popup_finger->close();
                      });
                  connect(annuler, &QPushButton::clicked, [=]() {
                      on_remove_clicked();
                      popup_finger->close();
                  });

                  popup_finger->exec();


          // Fermeture de la popup
          delete popup_finger;
}
}





void MainWindow::on_role_currentIndexChanged(int index)
{
    index = ui->role->currentIndex();
    QString role_de_index = ui->role->itemText(index);
    if (role_de_index=="Infirmier")
    {
         ui->medecin->show();
    }
}

void MainWindow::on_remove_clicked()
{
    // Création d'une nouvelle fenêtre popup
    QDialog *popup_delete_user = new QDialog(this);
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(Qt::white));
    popup_delete_user->setPalette(palette);

     QLabel *messageLabel = new QLabel(popup_delete_user);
     messageLabel->setText("Voulez-vous vraiment supprimer cet utilisateur ?");
     messageLabel->setAlignment(Qt::AlignCenter);
    // Définition du titre et de la taille de la popup
    popup_delete_user->setWindowTitle("Supprimer");
    popup_delete_user->setFixedSize(400, 300);



    QPushButton *validerBtn = new QPushButton("Valider", popup_delete_user);
     QVBoxLayout *layout = new QVBoxLayout(popup_delete_user);
     layout->addWidget(messageLabel);
     layout->addWidget(validerBtn);

    connect(validerBtn, &QPushButton::clicked, [=]() {
        if(!connOpen())
        {
            qDebug() << "Failed to open the Database";
            return;
        }
     QSqlQuery remove_user;
     remove_user.prepare("DELETE  FROM USER where USERNAME='"+nom_utilisateur_line_edit+"'");
     if (!remove_user.exec()) {
         qDebug() << "Failed to execute query";
         qDebug() << remove_user.lastError().text();
     }

     popup_delete_user->close();

     });
connClose();
 // Affichage de la popup
 popup_delete_user->exec();

 // Libération de la
 delete validerBtn;
 delete layout;
 delete popup_delete_user;

}

void MainWindow::on_modify_clicked()
{
// Création d'une nouvelle fenêtre popup
QDialog *popup_modify_user = new QDialog(this);
QPalette palette;
palette.setColor(QPalette::Window, QColor(Qt::white));
popup_modify_user->setPalette(palette);

popup_modify_user->setWindowTitle("Modifier");
popup_modify_user->setFixedSize(300, 300);
//    popup_modify_user->setStyleSheet("border-radius: 20px;");


QLabel* nameLabel = new QLabel("Nom d'utilisateur:");
QLineEdit* nameLineEdit = new QLineEdit(popup_modify_user);
nameLineEdit->setFixedSize(150,20);
QLabel* passwordLabel = new QLabel("Mot de passe:");
QLineEdit* passwordLineEdit = new QLineEdit(popup_modify_user);
passwordLineEdit->setFixedSize(150,20);
//QLabel* fonctionLabel = new QLabel("Fonction:");
//QLineEdit* fonctionLineEdit = new QLineEdit(popup_modify_user);
//fonctionLineEdit->setFixedSize(150,20);

// Création d'un bouton pour fermer la popup
QPushButton *validerBtn = new QPushButton("Valider", popup_modify_user);

QPushButton *annulerBtn = new QPushButton("Annuler", popup_modify_user);

// Création d'un layout pour organiser les widgets
QVBoxLayout *layout = new QVBoxLayout(popup_modify_user);
layout->addWidget(nameLabel,0, Qt::AlignHCenter | Qt::AlignTop);
layout->addWidget(nameLineEdit,0, Qt::AlignHCenter | Qt::AlignTop);
layout->addWidget(passwordLabel,0, Qt::AlignHCenter | Qt::AlignTop);
layout->addWidget(passwordLineEdit,0, Qt::AlignHCenter | Qt::AlignTop);
//layout->addWidget(fonctionLabel,0, Qt::AlignHCenter | Qt::AlignTop);
//layout->addWidget(fonctionLineEdit,0, Qt::AlignHCenter | Qt::AlignTop);
layout->addWidget(validerBtn,0, Qt::AlignHCenter | Qt::AlignTop);
layout->addWidget(annulerBtn,0, Qt::AlignHCenter | Qt::AlignBottom);

connect(annulerBtn, &QPushButton::clicked, popup_modify_user, &QDialog::close);

// Connexion du clic sur le bouton "fermer" à la fonction d'enregistrement des données
connect(validerBtn, &QPushButton::clicked, [=]() {

    // Récupération des valeurs des champs d'entrée
    QString modify_username_user= nameLineEdit->text();
    QString modify_password_user =passwordLineEdit->text();
//    QString modify_role_user= fonctionLineEdit->text();
connOpen();
if(modify_username_user.isEmpty()&& !modify_password_user.isEmpty())
{
 QSqlQuery modify_user;
 modify_user.prepare("UPDATE USER SET  PASSWORD='" + modify_password_user + "' WHERE USERNAME=:user");
         modify_user.bindValue(":user", username_for_delete);
         if (!modify_user.exec()) {
             qDebug() << "Failed to execute query";
             qDebug() << modify_user.lastError().text();
         }
}
else if (!modify_username_user.isEmpty()&& modify_password_user.isEmpty())
{
    QSqlQuery modify_user;
    modify_user.prepare("UPDATE USER SET USERNAME='" +modify_username_user+ "'  WHERE USERNAME=:user");
            modify_user.bindValue(":user", username_for_delete);
            if (!modify_user.exec()) {
                qDebug() << "Failed to execute query";
                qDebug() << modify_user.lastError().text();
            }
}
else if (!modify_username_user.isEmpty()&& !modify_password_user.isEmpty())
{
    QSqlQuery modify_user;
    modify_user.prepare("UPDATE USER SET USERNAME='" +modify_username_user+ "', PASSWORD='" + modify_password_user + "'  WHERE USERNAME=:user");
            modify_user.bindValue(":user", username_for_delete);
            if (!modify_user.exec()) {
                qDebug() << "Failed to execute query";
                qDebug() << modify_user.lastError().text();
            }
}
connClose();
 popup_modify_user->close();

});

// Affichage de la popup
popup_modify_user->exec();

// Libération de la
delete validerBtn;
delete layout;
delete popup_modify_user;
}




