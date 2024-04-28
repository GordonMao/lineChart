/*******************************************************************
 This file is part of Synkron
 Copyright (C) 2005-2009 Matus Tomlein (matus.tomlein@gmail.com)

 Synkron is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public Licence
 as published by the Free Software Foundation; either version 2
 of the Licence, or (at your option) any later version.

 Synkron is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public Licence for more details.

 You should have received a copy of the GNU General Public Licence
 along with Synkron; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
********************************************************************/

#include "mainwindow.h"
//#include "widget.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCore/QDateTime>
#include <QtCharts/QDateTimeAxis>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>
#include <QtCharts/QValueAxis>




int SyncPage::localFuncAddWidgetSyncfolders()
{

#if 1
    folders_hlayout = new QHBoxLayout;
    add_folder_btn = new QPushButton (this);
    add_folder_btn->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/add.png")));
    add_folder_btn->setStatusTip(tr("Add"));
    add_folder_btn->setFlat(true);
    add_folder_btn->setMinimumSize(22, 22);
    add_folder_btn->setMaximumSize(22, 22);
    folders_hlayout->addWidget(add_folder_btn);
#endif


    show_sync_folders = new QCheckBox (tab);
    show_sync_folders->setStatusTip(tr("Show/hide sync folders"));
    show_sync_folders->setChecked(true);

    folders_hlayout->addWidget(show_sync_folders);
    QLabel * sync_text = new QLabel (tab);
    sync_text->setText(tr("<b>Sync folders:</b>"));
    folders_hlayout->addWidget(sync_text);
    folders_hlayout->addStretch();
    mainglayout->addLayout(folders_hlayout, 1, 0); // ###################################


    sync_folders = new SyncFolders (this);
    connect(sync_folders, SIGNAL(sigfolderschanged()), this, SLOT(syncFoldersChanged()));
    connect(sync_folders, SIGNAL(sigfoldersedited()), this, SLOT(syncFoldersEdited()));
    connect(show_sync_folders, SIGNAL(toggled(bool)), sync_folders, SLOT(setVisible(bool)));
    connect(show_sync_folders, SIGNAL(toggled(bool)), add_folder_btn, SLOT(setVisible(bool)));
    connect(add_folder_btn, SIGNAL(released()), sync_folders, SLOT(addFolder()));
    mainglayout->addWidget(sync_folders, 2, 0);



    return 0;
}

int SyncPage::localFuncAddWidget2()
{
#if 0
    sync_folders = new SyncFolders (this);
    connect(sync_folders, SIGNAL(sigfolderschanged()), this, SLOT(syncFoldersChanged()));
    connect(sync_folders, SIGNAL(sigfoldersedited()), this, SLOT(syncFoldersEdited()));
    connect(show_sync_folders, SIGNAL(toggled(bool)), sync_folders, SLOT(setVisible(bool)));
    connect(show_sync_folders, SIGNAL(toggled(bool)), add_folder_btn, SLOT(setVisible(bool)));
    connect(add_folder_btn, SIGNAL(released()), sync_folders, SLOT(addFolder()));
    mainglayout->addWidget(sync_folders, 2, 0);
#endif
    return 0;
}


int SyncPage::localFuncAddWidgetSynclog()
{
#if 1
    QGridLayout * hlayout3 = new QGridLayout;
    sync_log_label = new QLabel (tab);
    sync_log_label->setText(tr("<b>Sync log:</b>"));
    hlayout3->addWidget(sync_log_label, 0, 0);
    spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hlayout3->addItem(spacerItem, 0, 1);
    log_search = new ExtendedLineEdit(tab);
    log_search->setStatusTip(tr("Search sync log"));
    log_search->setText(tr("Search"));
    QObject::connect(log_search, SIGNAL(textEdited(const QString &)), mp_parent, SLOT(searchTw(const QString)));
    QObject::connect(log_search, SIGNAL(returnPressed()), this, SLOT(searchAnalyseTree()));
    hlayout3->addWidget(log_search, 0, 3);


    mainglayout->addLayout(hlayout3, 5, 0);// #################################
#endif

    tw = new QTableWidget (0, 2, tab);
    tw->setHorizontalHeaderLabels(QStringList() << tr("Source") << tr("Destination"));
    tw->verticalHeader()->hide();
    tw->setMinimumSize(0, 50);
    tw->setShowGrid(false);
    tw->setStatusTip(tr("List of synchronised files and folders"));
#if QT_VERSION >= 0x050000
    tw->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#else
    tw->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#endif

    tw->setLayoutDirection(Qt::LeftToRight);
    status_table_item = new QTableWidgetItem(tr("Press the \"Sync\" button to start synchronisation"));
    tw->insertRow(0);
    status_table_item->setBackground(Qt::blue);
    status_table_item->setForeground(Qt::white);
    tw->setSpan(0, 0, 1, 2);
    tw->setItem(0, 0, status_table_item);
    tw->setRowHeight(0, 16);
    tw->setTextElideMode(Qt::ElideMiddle);

    analyse_tree = new ExtendedTreeWidget (this);
    analyse_tree->setAlternatingRowColors(true);
    analyse_tree->setUniformRowHeights(true);
    connect(analyse_tree, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(analyseTreeItemClicked(QTreeWidgetItem *, int)));
    connect(analyse_tree, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(analyseTreeItemDoubleClicked(QTreeWidgetItem *, int)));
    connect(analyse_tree, SIGNAL(sigconmenu(QPoint)), this, SLOT(analyseTreeConMenu(QPoint)));
    connect(analyse_tree, SIGNAL(itemExpanded(QTreeWidgetItem *)), this, SLOT(analyseTreeItemExpanded(QTreeWidgetItem *)));

    logs_stw = new QStackedWidget (tab);
    logs_stw->addWidget(tw);
    logs_stw->addWidget(analyse_tree);

    mainglayout->addWidget(logs_stw, 5, 0); // ###################################

    return 0;
}

int SyncPage::localFuncAddWidgetAdvance()
{
    QHBoxLayout * hlayout4 = new QHBoxLayout;
    show_advanced = new QPushButton(tr("Advanced") + "  ", tab);
    show_advanced->setStatusTip(tr("Show advanced options of configuration"));
    show_advanced->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/configure16.png")));
    hlayout4->addWidget(show_advanced);
    hlayout4->addStretch(); // ###################################

    go_to_analyse = new QPushButton (tr("Analyse"), tab);
    go_to_analyse->setStatusTip(tr("Analyse"));
    go_to_analyse->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/analyse_16.png")));
    connect(go_to_analyse, SIGNAL(released()), this, SLOT(goToAnalyse()));
    hlayout4->addWidget(go_to_analyse);

    sync_btn = new QPushButton (tr("Sync"), tab);
    sync_btn->setStatusTip(tr("Synchronise"));
    sync_btn->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/sync22.png")));
    QObject::connect(sync_btn, SIGNAL(released()), this, SLOT(sync()));
    QObject::connect(this, SIGNAL(sigsync(QWidget *)), this, SLOT(sync()));
    hlayout4->addWidget(sync_btn);

    stop_sync_btn = new QPushButton (tr("Stop sync"), tab);
    stop_sync_btn->setStatusTip(tr("Stop synchronisation"));
    stop_sync_btn->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/stop_sync22.png")));
    stop_sync_btn->setVisible(false);
    hlayout4->addWidget(stop_sync_btn);
    QObject::connect(stop_sync_btn, SIGNAL(released()), this, SLOT(stopSync()));
    mainglayout->addLayout(hlayout4, 6, 0); // ##################################

    return 0;
}


int SyncPage::localFuncAddWidget5()
{

    return 0;
}


int SyncPage::localFuncAddWidgetLine()
{
#if 1    
    do
    {
        int filelines = 0;
        int found = 0;
        QString titleV;
        QString titleNUM;
        int xCount = 0;    
        //![1]
        QHBoxLayout * hlayoutqc = new QHBoxLayout;
        
        QLineSeries *series = new QLineSeries();
        QLineSeries *seriesMcs = new QLineSeries();
        QLineSeries *series2 = new QLineSeries();
        //![1]
#if 0
        //![2]
        // data from http://www.swpc.noaa.gov/ftpdir/weekly/RecentIndices.txt
        // http://www.swpc.noaa.gov/ftpdir/weekly/README
        // http://www.weather.gov/disclaimer
        QFile sunSpots(":sun");
        if (!sunSpots.open(QIODevice::ReadOnly | QIODevice::Text)) 
        {
            break;
        }
    
    /*
    
        while (iter.hasNext()) {
            iter.next();
            if (path.startsWith(iter.key())) 
            {
                path.replace(0, iter.key().length(), iter.value());
            }
        }
    
        */
#define lineFact (130)
#define MAX_X (2000)
    
        QTextStream stream(&sunSpots);
        while (!stream.atEnd()) 
        {
            found = 0;
            QString line = stream.readLine();
//            if (line.startsWith("#") || line.startsWith(":"))
//                continue;

            QStringList values = line.split(" ", QString::SkipEmptyParts);
            for (int i = 1; i < values.count(); ++i) 
            {
                if (values[i].startsWith("dlSchd_lst1")) 
                {
    //                values.startsWith(iter.key())
                    series->append(xCount, 4.5);
    
                }
    
                if ( values[i].startsWith("dlSchd_lst2") ) 
                {
    //                mcsAfterCodeControl[0]:28
    //                seriesMcs->append(filelines, values[i+7]);
    
                    seriesMcs->append(xCount, 3.5);
                }
    
                if (values[i].startsWith("dlSchd_lst3")) 
                {
    //                found = 1;
                    found = xCount;
                    series2->append(xCount, 1.5);
    
                }
    
                if ((found) == xCount)
                {
                    xCount++;
                }
                
        //        values.at(i);
            }


            if ((found+1) == xCount)
            {
                xCount++;
                series->append(xCount, 0.5);
            }

    
#if 0
            QDateTime momentInTime;
            momentInTime.setDate(QDate(values[0].toInt(), values[1].toInt() , 15));
            series->append(momentInTime.toMSecsSinceEpoch(), values[2].toDouble());
#endif
            filelines++;
            if (filelines>MAX_X)
                break;            
        }
        sunSpots.close();
        //![2]
#endif


        for (int i = 1; i < 100; ++i) 
        {
            series->append(xCount, 4.5);

            seriesMcs->append(xCount, 3.5);

            series2->append(xCount, 1.5);

            if ((found) == xCount)
            {
                xCount++;
            }
            
        }

    
        //![3]
        titleV += "dlSchd_lst1 ";
     //   titleNUM.setNum(filelines,10);
     // titleV += titleNUM;
        
        titleV += QString::number(filelines,10);    
        
        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->addSeries(seriesMcs);
        chart->addSeries(series2);


        chart->legend()->hide();
        chart->setTitle(titleV);
        
        
        //![3]
    
        //![4]
    //      QDateTimeAxis *axisX = new QDateTimeAxis;
    //      axisX->setTickCount(10);
    //      axisX->setFormat("hh mm ss");
    //      axisX->setTitleText("time");
#if 1
        QValueAxis *axisX = new QValueAxis;
        axisX->setLabelFormat("%f");
        axisX->setTitleText("slot");
        axisX->setTickCount(10);//设置刻度线的数量
        axisX->setTickInterval(1);//设置刻度的间隔
#else
        QDateTimeAxis *axisX = new QDateTimeAxis;
         axisX->setTickCount(10);
         axisX->setFormat("MMM yyyy");
         axisX->setTitleText("Date");
         chart->addAxis(axisX, Qt::AlignBottom);
         series->attachAxis(axisX);
    
#endif
    
        chart->addAxis(axisX, Qt::AlignBottom);//给chart添加坐标系，对齐底部，即x轴
        series->attachAxis(axisX);//序列关联到x轴
        seriesMcs->attachAxis(axisX);//序列关联到x轴
        series2->attachAxis(axisX);//序列关联到x轴
    
        QValueAxis *axisY = new QValueAxis;
        axisY->setLabelFormat("%f");
        axisY->setTitleText("dlSch");
        axisY->setTickCount(5);//设置刻度线的数量
        
        chart->addAxis(axisY, Qt::AlignLeft);
        
        series->attachAxis(axisY);
        seriesMcs->attachAxis(axisY);//序列关联到y轴
        series2->attachAxis(axisY);//序列关联到y轴
        //![4]
    
        //![5]
        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);//函数可以启动反走样
        chartView->chart()->setTheme(QChart::ChartThemeBlueCerulean);//设置主题样式
        
        mchartView = chartView;
        //![5]

        hlayoutqc->addWidget(chartView);
        //hlayoutqc->heightForWidth(50);
        
        mainglayout->addLayout(hlayoutqc, 4, 0); // ##################################

        updateTimer2 = new QTimer(this);
        connect(updateTimer2, &QTimer::timeout, this, &SyncPage::updateRotationLine);
        updateTimer2->start(1250);
        
    }
    while (0);



#endif        

    return 0;
}




int SyncPage::localFuncAddWidgetPai()
{

#if 1
    
    //    QChartView * chartView = new QChartView;
    
    {
//        setMinimumSize(800, 600);
    
        //! [1]
//        QGridLayout * hlayoutqc = new QGridLayout;
        QHBoxLayout * hlayoutqc = new QHBoxLayout;
        
        QChartView *chartView = new QChartView(tab);
        chartView->setRenderHint(QPainter::Antialiasing);
        QChart *chart = chartView->chart();
        chart->legend()->setVisible(false);
        chart->setTitle("Nested donuts demo");
        chart->setAnimationOptions(QChart::AllAnimations);
        hlayoutqc->heightForWidth(50);
        //! [1]
    
        //! [2]
        qreal minSize = 0.2;
        qreal maxSize = 2;
        int donutCount = 10;
        //! [2]
    
        //! [3]
        for (int i = 0; i < donutCount; i++) 
        {
            QPieSeries *donut = new QPieSeries;
            int sliceCount =  1;//3 + QRandomGenerator::global()->bounded(3);
            for (int j = 0; j < sliceCount; j++) 
            {
                qreal value = 80;// 100 + QRandomGenerator::global()->bounded(100);
#if 1
                QPieSlice *slice = new QPieSlice(QString("%1").arg(value), value);
                slice->setLabelVisible(true);
                slice->setLabelColor(Qt::white);
                slice->setLabelPosition(QPieSlice::LabelInsideTangential);
                
                //connect(slice, &QPieSlice::hovered, this, &SyncPage::explodeSlice);
                donut->append(slice);
#endif                
                donut->setHoleSize(minSize + i * (maxSize - minSize) / donutCount);//洞的大小
                donut->setPieSize(minSize + (i + 1) * (maxSize - minSize) / donutCount);//园的大小
                //donut->setPieSize(0.5);//园的大小

                donut->setPieStartAngle(0 );
                donut->setPieEndAngle(0);
                
            }
            m_donuts.append(donut);
            chartView->chart()->addSeries(donut);
        }
        //! [3]
    
        // create main layout
        //! [4]
//        QGridLayout *mainLayout = new QGridLayout;
#if 1
        hlayoutqc->addWidget(chartView);
        mainglayout->addLayout(hlayoutqc, 4, 0); // ##################################
#else
        mainglayout->addWidget(hlayoutqc, 1, 1);
//        setLayout(mainLayout);
#endif        
        //! [4]
#if 1    
        //! [5]
        
        updateTimer = new QTimer(this);
        connect(updateTimer, &QTimer::timeout, this, &SyncPage::updateRotationPai);
        updateTimer->start(1250);
#endif        
        //! [5]
    }
#endif        
    return 0;
}




int SyncPage::localFuncAddWidgetAdvanceSub()
{
        //Advanced -----------------------------------------------------------------
        advanced_menu = new QMenu;
        advanced_menu->setWindowTitle(tr("Advanced"));
        advanced_menu->setTearOffEnabled(true);
    
        sync_hidden = new QAction (tab);
        sync_hidden->setCheckable(true);
        sync_hidden->setStatusTip(tr("Synchronise hidden files and folders"));
        sync_hidden->setText(tr("Synchronise hidden files and folders"));
        advanced_menu->addAction(sync_hidden);
    
        no_empty_folders = new QAction (tab);
        no_empty_folders->setCheckable(true);
        no_empty_folders->setStatusTip(tr("Do not create empty folders"));
        no_empty_folders->setText(tr("Do not create empty folders"));
        advanced_menu->addAction(no_empty_folders);
    
        sync_nosubdirs = new QAction (tab);
        sync_nosubdirs->setCheckable(true);
        sync_nosubdirs->setStatusTip(tr("Do not synchronise subdirectories"));
        sync_nosubdirs->setText(tr("Do not synchronise subdirectories"));
        advanced_menu->addAction(sync_nosubdirs);
    
        backup_folders = new QAction (tr("Do not backup updated files"), tab);
        backup_folders->setStatusTip(tr("Do not backup updated files"));
        backup_folders->setCheckable(true);
        advanced_menu->addAction(backup_folders);
    
        update_only = new QAction (tr("Update existing files only"), tab);
        update_only->setStatusTip(tr("Update existing files only"));
        update_only->setCheckable(true);
        advanced_menu->addAction(update_only);
    
        ignore_blacklist = new QAction (tab);
        ignore_blacklist->setCheckable(true);
        ignore_blacklist->setStatusTip(tr("Ignore blacklist"));
        ignore_blacklist->setText(tr("Ignore blacklist"));
        connect(ignore_blacklist, SIGNAL(toggled(bool)), this, SLOT(ignoreBlacklistClicked(bool)));
        advanced_menu->addAction(ignore_blacklist);
    
        allow_DST = new QAction (tab);
        allow_DST->setCheckable(true);
        allow_DST->setText(tr("Ignore 1 hour difference"));
        allow_DST->setStatusTip(tr("Due to Daylight Saving Time (DST), files may have 1 hour delta"));
        advanced_menu->addAction(allow_DST);
    
        move = new QAction (tr("Move contents to folder 2, leaving folder 1 empty"), tab);
        move->setCheckable(true);
        move->setStatusTip(tr("Move contents to folder 2, leaving folder 1 empty"));
        connect(move, SIGNAL(toggled(bool)), this, SLOT(moveStateChanged(bool)));
        advanced_menu->addAction(move);
        advanced_menu->addSeparator();
    
        propagate_deletions = new QAction (tab);
        propagate_deletions->setCheckable(true);
        propagate_deletions->setStatusTip(tr("Propagate deletions"));
        propagate_deletions->setText(tr("Propagate deletions"));
        connect(propagate_deletions, SIGNAL(toggled(bool)), this, SLOT(propagatedStateChanged(bool)));
        connect(propagate_deletions, SIGNAL(toggled(bool)), this, SLOT(propagatedClicked(bool)));
        advanced_menu->addAction(propagate_deletions);
    
        alert_collisions = new QAction (tab);
        alert_collisions->setCheckable(true);
        alert_collisions->setText(tr("Detect collisions"));
        alert_collisions->setStatusTip(tr("Detect and alert collided files"));
        connect(alert_collisions, SIGNAL(toggled(bool)), this, SLOT(propagatedStateChanged(bool)));
        connect(alert_collisions, SIGNAL(toggled(bool)), this, SLOT(propagatedClicked(bool)));
        advanced_menu->addAction(alert_collisions);
    
        text_database_action = new QAction (tab);
        text_database_action->setCheckable(true);
        text_database_action->setChecked(true);
        text_database_action->setText(tr("Store database in a text file"));
        text_database_action->setStatusTip(tr("Store database in a text file"));
        connect(text_database_action, SIGNAL(toggled(bool)), this, SLOT(useTextDatabase(bool)));
        advanced_menu->addAction(text_database_action);
    
        symlinks = new QAction(tab);
        symlinks->setCheckable(true);
#ifdef Q_WS_WIN
        symlinks->setChecked(false);
#else
        symlinks->setChecked(false);
        symlinks->setStatusTip(tr("Follow symbolic links"));
        symlinks->setText(tr("Follow symbolic links"));
        advanced_menu->addAction(symlinks);
#endif
    
        advanced_menu->addSeparator();
        QMenu * adv_analysis_menu = new QMenu;
        adv_analysis_menu->setTitle(tr("Analysis"));
        adv_analysis_menu->setStatusTip(tr("Advanced options for the analysis"));
        adv_analysis_menu->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/analyse_16.png")));
    
        fast_analyse = new QAction (tr("Fast analysis"), tab);
        fast_analyse->setCheckable(true);
        fast_analyse->setStatusTip(tr("Fast analysis"));
        adv_analysis_menu->addAction(fast_analyse);
    
        analyse_special_only = new QAction (tr("List files which need to be synchronised only"), tab);
        analyse_special_only->setCheckable(true);
        analyse_special_only->setStatusTip(tr("List files which need to be synchronised only"));
        adv_analysis_menu->addAction(analyse_special_only);
    
        sort_analysis_by_action = new QAction (tr("Sort by action"), tab);
        sort_analysis_by_action->setCheckable(true);
        sort_analysis_by_action->setStatusTip(tr("Sort by action"));
        adv_analysis_menu->addAction(sort_analysis_by_action);
    
        advanced_menu->addMenu(adv_analysis_menu);
    
        filters_menu = new QMenu;
        filters_menu->setWindowTitle(tr("Filters"));
        filters_menu->setTitle(tr("Filters"));
        filters_menu->setStatusTip(tr("Choose filters to be used"));
        filters_menu->setTearOffEnabled(true);
        filters_menu->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/filter16.png")));
        QAction * action;
        for (int f = 0; f < mp_parent->filter_list->count(); ++f) 
        {
            action = new QAction (mp_parent->filter_list->item(f)->text(), filters_menu);
            action->setCheckable(true);
            filters_menu->addAction(action);
        }
    
        advanced_menu->addMenu(filters_menu);
        advanced_menu->addSeparator();
    
        QAction * edit_blacklist = new QAction(tab);
        edit_blacklist->setText(tr("Edit blacklist"));
        edit_blacklist->setStatusTip(tr("Edit blacklist for this tab"));
        edit_blacklist->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/blacklist.png")));
        connect(edit_blacklist, SIGNAL(triggered(bool)), this, SLOT(editBlacklist()));
        advanced_menu->addAction(edit_blacklist);
    
        QAction * change_ignorance = new QAction(tab);
        change_ignorance->setText(tr("Change allowed time difference"));
        change_ignorance->setStatusTip(tr("Change the allowed time difference between synchronised files"));
        change_ignorance->setIcon(QIcon(QString::fromUtf8(":/new/prefix1/images/sync.png")));
        connect(change_ignorance, SIGNAL(triggered(bool)), this, SLOT(changeAllowedDifference()));
        advanced_menu->addAction(change_ignorance);


    show_advanced->setMenu(advanced_menu);

    return 0;
}






void SyncPage::updateRotationPai()
{
    for (int i = 0; i < m_donuts.count(); i++) 
    {
        QPieSeries *donut = m_donuts.at(i);
//      qreal phaseShift =  -50 + QRandomGenerator::global()->bounded(100);
    
        qreal phaseShift =  5 ;
//        donut->setPieStartAngle(donut->pieStartAngle() + phaseShift);
//        donut->setPieEndAngle(donut->pieEndAngle() + phaseShift);
        donut->setPieStartAngle(donut->pieStartAngle() );
        if (donut->pieEndAngle()<360)
        {
            donut->setPieEndAngle(donut->pieEndAngle() + phaseShift);
        }
        else
        {
            donut->setPieEndAngle(0);
        }
    }
}




void SyncPage::updateRotationLine()
{

//    emit sigsync(tab);
//QChart::ChartThemeQt
    //static int theme=0;
    static QChart::ChartTheme emTheme=QChart::ChartThemeLight;
    //emTheme++;

    mchartView->chart()->setTheme(emTheme);
    if (emTheme == QChart::ChartThemeQt )
    {
        emTheme = QChart::ChartThemeLight;
    }
    else if (emTheme == QChart::ChartThemeLight )
    {
        emTheme = QChart::ChartThemeBlueCerulean;
    }
    else if (emTheme == QChart::ChartThemeBlueCerulean )
    {
        emTheme = QChart::ChartThemeDark;
    }
    else if (emTheme == QChart::ChartThemeDark )
    {
        emTheme = QChart::ChartThemeBrownSand;
    }
    else if (emTheme == QChart::ChartThemeBrownSand )
    {
        emTheme = QChart::ChartThemeBlueNcs;
    }
    else if (emTheme == QChart::ChartThemeBlueNcs )
    {
        emTheme = QChart::ChartThemeHighContrast;
    }
    else if (emTheme == QChart::ChartThemeHighContrast )
    {
        emTheme = QChart::ChartThemeBlueIcy;
    }
    else if (emTheme == QChart::ChartThemeBlueIcy )
    {
        emTheme = QChart::ChartThemeQt;
    }
    
}

#if 1
void SyncPage::explodeSlice(bool exploded)

{
#if 0
    QPieSlice *slice = qobject_cast<QPieSlice *>(sender());

    if (exploded) 
    {
        updateTimer->stop();
        qreal sliceStartAngle = 0;slice->startAngle();
        qreal sliceEndAngle   = 90;//slice->startAngle() + slice->angleSpan();

        QPieSeries *donut = slice->series();
        qreal seriesIndex = m_donuts.indexOf(donut);

        for (int i = seriesIndex + 1; i < m_donuts.count(); i++) 
        {
            m_donuts.at(i)->setPieStartAngle(sliceEndAngle);
            m_donuts.at(i)->setPieEndAngle(180 + sliceStartAngle);
        }
    } 
    else 
    {
        for (int i = 0; i < m_donuts.count(); i++) 
        {
            m_donuts.at(i)->setPieStartAngle(0);
            m_donuts.at(i)->setPieEndAngle(180);
        }
        updateTimer->start();
    }
    slice->setExploded(exploded);
#endif
}

#else
void SyncPage::explodeSlice(bool exploded)

{
    QPieSlice *slice = qobject_cast<QPieSlice *>(sender());

    if (exploded) 
    {
        updateTimer->stop();
        qreal sliceStartAngle = slice->startAngle();
        qreal sliceEndAngle   = slice->startAngle() + slice->angleSpan();

        QPieSeries *donut = slice->series();
        qreal seriesIndex = m_donuts.indexOf(donut);

        for (int i = seriesIndex + 1; i < m_donuts.count(); i++) 
        {
            m_donuts.at(i)->setPieStartAngle(sliceEndAngle);
            m_donuts.at(i)->setPieEndAngle(360 + sliceStartAngle);
        }
    } 
    else 
    {
        for (int i = 0; i < m_donuts.count(); i++) 
        {
            m_donuts.at(i)->setPieStartAngle(0);
            m_donuts.at(i)->setPieEndAngle(360);
        }
        updateTimer->start();
    }
    slice->setExploded(exploded);
}
#endif



