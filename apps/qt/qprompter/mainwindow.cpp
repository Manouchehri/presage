/****************************************************************************
 **
 ** Copyright (C) 2011 Matteo Vescovi <matteo.vescovi@yahoo.co.uk>
 ** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Nokia Corporation (qt-info@nokia.com)
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
 **     the names of its contributors may be used to endorse or promote
 **     products derived from this software without specific prior written
 **     permission.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

//! [0]
#include "mainwindow.h"

#include <QtGui>

#if QT_VERSION >= 0x050000
    #include <QtWidgets/QFileDialog>
    #include <QtWidgets/QMessageBox>
    #include <QtWidgets/QAction>
    #include <QtWidgets/QMenuBar>
    #include <QtWidgets/QMenu>
    #include <QtWidgets/QToolBar>
    #include <QtWidgets/QStatusBar>
    #include <QtWidgets/QApplication>
#endif

#include <Qsci/qsciscintilla.h>
//! [0]

MainWindow::QsciScintillaPresageCallback::QsciScintillaPresageCallback(QsciScintilla* sci)
{
    qsci = sci;
}

MainWindow::QsciScintillaPresageCallback::~QsciScintillaPresageCallback()
{
    // complete
}

const long MainWindow::QsciScintillaPresageCallback::max_callback_context_range_size = 2048;

std::string
MainWindow::QsciScintillaPresageCallback::get_past_stream() const {
    long curpos = qsci->SendScintilla(QsciScintillaBase::SCI_GETCURRENTPOS);

    long startpos = curpos - max_callback_context_range_size;
    if (startpos < 0)
    {
	startpos = 0;
    }

    char *buf = new char[curpos - startpos + 1];
    
    qsci->SendScintilla(QsciScintillaBase::SCI_GETTEXTRANGE, startpos, curpos, buf);
    
    std::string result = buf;
    delete[] buf;
    return result;
}

std::string
MainWindow::QsciScintillaPresageCallback::get_future_stream() const {
    long curpos = qsci->SendScintilla(QsciScintillaBase::SCI_GETCURRENTPOS);
    long endpos = qsci->SendScintilla(QsciScintillaBase::SCI_GETTEXTLENGTH);
    
    if (endpos > curpos + max_callback_context_range_size)
    {
	endpos = curpos + max_callback_context_range_size;
    }

    char *buf = new char[endpos - curpos + 1];
    
    qsci->SendScintilla(QsciScintillaBase::SCI_GETTEXTRANGE, curpos, endpos, buf);
    
    std::string result = buf;
    delete[] buf;
    return result;
}


//! [1]
MainWindow::MainWindow()
//! [1] //! [2]
{
    textEdit = new QsciScintilla;
    textEdit->setMarginWidth(0, 0);
    textEdit->setMarginWidth(1, 0);
    textEdit->setMarginWidth(2, 0);
    textEdit->setMarginWidth(3, 0);
    textEdit->setMarginWidth(4, 0);
    setCentralWidget(textEdit);

    qsci_prsg_cb = new QsciScintillaPresageCallback(textEdit);
    prsg = new Presage(qsci_prsg_cb);

    /* set autocompletion box height
    std::string value = prsg->config ("Presage.Selector.SUGGESTIONS");
    int height = atoi (value.c_str());
    textEdit->SendScintilla(QsciScintillaBase::SCI_AUTOCSETMAXHEIGHT, height);
    */

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    readSettings();

    connect(textEdit, SIGNAL(textChanged()),
            this, SLOT(documentWasModified()));

    connect(textEdit, SIGNAL(SCN_CHARADDED(int)),
	    this, SLOT(handleCharAdded(int)));

    connect(textEdit, SIGNAL(SCN_UPDATEUI(int)),
	    this, SLOT(showPrediction()));

    connect(textEdit, SIGNAL(userListActivated(int, const QString &)),
	    this, SLOT(handleUserListSelection(int, const QString &)));

    setCurrentFile("");
    setUnifiedTitleAndToolBarOnMac(true);
}
//! [2]

//! [3]
void MainWindow::closeEvent(QCloseEvent *event)
//! [3] //! [4]
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}
//! [4]

//! [5]
void MainWindow::newFile()
//! [5] //! [6]
{
    if (maybeSave()) {
        textEdit->clear();
        setCurrentFile("");
    }
}
//! [6]

//! [7]
void MainWindow::open()
//! [7] //! [8]
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}
//! [8]

//! [9]
bool MainWindow::save()
//! [9] //! [10]
{
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}
//! [10]

//! [11]
bool MainWindow::saveAs()
//! [11] //! [12]
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}
//! [12]

//! [13]
void MainWindow::about()
//! [13] //! [14]
{
    QMessageBox::about(this,
		       tr("About qprompter"),
		       tr("This program is intended as a demonstration of Presage ONLY.\n"
			  "The Presage project aims to provide an intelligent predictive text entry "
			  "platform. Its intent is NOT to provide a predictive text entry user interface.\n"
			  "\n"
			  "Think of Presage as the predictive backend that sits behind a shiny user "
			  "interface and does all the predictive heavy lifting.\n"
			  "\n"
			  "Copyright (C) Matteo Vescovi\n"
			  "http://presage.sourceforge.net\n"
			   )
	);
}
//! [14]

//! [15]
void MainWindow::documentWasModified()
//! [15] //! [16]
{
    setWindowModified(textEdit->isModified());
}
//! [16]

void MainWindow::showPrediction()
{
    bool glob_function_keys_mode = true;

    //  QMessageBox::information(this, tr("Presage"),
    //			   tr("This should trigger a prediction."));

    if (textEdit->isListActive())
        textEdit->cancelList();
  
    std::vector<std::string> prediction = prsg->predict();

    QStringList list;
    for (size_t idx = 0;
	 idx < prediction.size();
	 idx++)
    {
        QString list_item;
        if (glob_function_keys_mode) {
            list_item = QString("F%1 ").arg(idx + 1);
        }
	list_item.append(prediction[idx].c_str());

        list << list_item;
    }

    textEdit->showUserList(1, list);
}

void MainWindow::handleUserListSelection(int id,
					 const QString &sel)
{
    bool glob_function_keys_mode = true;

    QString selection = sel;

    if (glob_function_keys_mode)
    {
	// strip function key from selection

        // find location of space char
        int idx = selection.indexOf(' ');
	if (-1 == idx)
	{
	    // this happens when prediction is empty
	    return;
	}

	selection.remove(0, idx + 1);
    }

    std::string selection_str = selection.toUtf8().constData();
    std::string completion_str = prsg->completion(selection_str);
    QString completion = QString::fromUtf8(completion_str.c_str());

    if (! completion.isEmpty())
    {
        //textEdit->insert(completion);
        textEdit->SendScintilla(QsciScintillaBase::SCI_ADDTEXT,
				completion_str.size(),
				completion_str.c_str());
    }

}

void MainWindow::handleCharAdded(int charadded)
{
    QString glob_autopunctuation_chars = ".,;:'?!$%&";
    QString glob_autopunctuation_whitespace = " ";

    /* if added char is an autopunctuation char */
    if (glob_autopunctuation_chars.contains(charadded))
    {
        qDebug() << "autopunctuation char: " << charadded;

        long curr_pos = textEdit->SendScintilla(QsciScintillaBase::SCI_GETCURRENTPOS);

	if (curr_pos > 1)
        {
            qDebug() << "current pos: " << curr_pos;

            long prev_pos = curr_pos - 2;
            long prev_char = textEdit->SendScintilla(QsciScintillaBase::SCI_GETCHARAT,
						     prev_pos);

	    qDebug() << "prev_pos: " << prev_pos;
	    qDebug() << "prev_char: " << prev_char;

            /* if previous char is a autopunctuation whitespace char */
            if (glob_autopunctuation_whitespace.contains(static_cast<int>(prev_char)))
            {
                qDebug() << "autopunctuation activated";

                char replacement[3] = { charadded, prev_char, 0 };

                textEdit->SendScintilla(QsciScintillaBase::SCI_SETSELECTIONSTART,
					prev_pos);

                textEdit->SendScintilla(QsciScintillaBase::SCI_SETSELECTIONEND,
					curr_pos);

                textEdit->SendScintilla(QsciScintillaBase::SCI_REPLACESEL,
					replacement);
            }
	}
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    bool glob_function_keys_mode = true;

    if (glob_function_keys_mode)
    {	
	if (textEdit->isListActive()) {
	    
	    if (event->key() >= Qt::Key_F1 && event->key() <= Qt::Key_F35)
	    {
		int fkn = event->key() - Qt::Key_F1 + 1;
		QString fk = QString("F%1 ").arg(fkn);

		textEdit->SendScintilla(QsciScintillaBase::SCI_AUTOCSELECT,
					fk.toUtf8().constData());

		textEdit->SendScintilla(QsciScintillaBase::SCI_AUTOCCOMPLETE);
		
	    }
	}
    }
    else
    {
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::toggleWordWrap()
{
    if (textEdit->wrapMode() == QsciScintilla::WrapNone)
    {
	textEdit->setWrapMode(QsciScintilla::WrapWord);
    }
    else
    {
	textEdit->setWrapMode(QsciScintilla::WrapNone);
    }
}

void MainWindow::invertColours()
{
    if (invertColoursAct->isChecked())
    {
	textEdit->SendScintilla(QsciScintillaBase::SCI_STYLESETFORE,
				QsciScintillaBase::STYLE_DEFAULT,
				(255 | 255 << 8 | 255 << 16));
	textEdit->SendScintilla(QsciScintillaBase::SCI_STYLESETBACK,
				QsciScintillaBase::STYLE_DEFAULT);
	textEdit->SendScintilla(QsciScintillaBase::SCI_STYLECLEARALL);

    }
    else
    {
	textEdit->SendScintilla(QsciScintillaBase::SCI_STYLESETFORE,
				QsciScintillaBase::STYLE_DEFAULT);
	textEdit->SendScintilla(QsciScintillaBase::SCI_STYLESETBACK,
				QsciScintillaBase::STYLE_DEFAULT,
				(255 | 255 << 8 | 255 << 16));
	textEdit->SendScintilla(QsciScintillaBase::SCI_STYLECLEARALL);

    }

}

//! [17]
void MainWindow::createActions()
//! [17] //! [18]
{
    newAct = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

//! [19]
    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
//! [18] //! [19]

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

//! [20]
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
//! [20]
    exitAct->setStatusTip(tr("Exit qprompter"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    undoAct = new QAction(tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last action"));
    connect(undoAct, SIGNAL(triggered()), textEdit, SLOT(undo()));

    redoAct = new QAction(tr("&Redo"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo the last undone action"));
    connect(redoAct, SIGNAL(triggered()), textEdit, SLOT(redo()));

//! [21]
    cutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
//! [21]
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, SIGNAL(triggered()), textEdit, SLOT(cut()));

    copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, SIGNAL(triggered()), textEdit, SLOT(copy()));

    pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, SIGNAL(triggered()), textEdit, SLOT(paste()));

    selectAllAct = new QAction(tr("Select &All"), this);
    selectAllAct->setShortcuts(QKeySequence::SelectAll);
    selectAllAct->setStatusTip(tr("Select the entire contents"));
    connect(selectAllAct, SIGNAL(triggered()), textEdit, SLOT(selectAll()));

    zoomInAct = new QAction(tr("Zoom &In"), this);
    zoomInAct->setShortcuts(QKeySequence::ZoomIn);
    zoomInAct->setStatusTip(tr("Zoom in"));
    connect(zoomInAct, SIGNAL(triggered()), textEdit, SLOT(zoomIn()));

    zoomOutAct = new QAction(tr("Zoom &Out"), this);
    zoomOutAct->setShortcuts(QKeySequence::ZoomOut);
    zoomOutAct->setStatusTip(tr("Zoom out"));
    connect(zoomOutAct, SIGNAL(triggered()), textEdit, SLOT(zoomOut()));

    wordWrapAct = new QAction(tr("Word &Wrap"), this);
    wordWrapAct->setShortcut(QKeySequence(tr("Ctrl+W")));
    wordWrapAct->setStatusTip(tr("Toggle line wrapping"));
    wordWrapAct->setCheckable(true);
    wordWrapAct->setChecked(true);
    textEdit->setWrapMode(QsciScintilla::WrapWord);
    connect(wordWrapAct, SIGNAL(triggered()), this, SLOT(toggleWordWrap()));

    invertColoursAct = new QAction(tr("&Invert Colours"), this);
    invertColoursAct->setShortcut(QKeySequence(tr("Ctrl+I")));
    invertColoursAct->setStatusTip(tr("Invert colours"));
    invertColoursAct->setCheckable(true);
    invertColoursAct->setChecked(false);
    connect(invertColoursAct, SIGNAL(triggered()), this, SLOT(invertColours()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the qprompter's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

//! [22]
    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
//! [22]

//! [23]
    cutAct->setEnabled(false);
//! [23] //! [24]
    copyAct->setEnabled(false);
    connect(textEdit, SIGNAL(copyAvailable(bool)),
            cutAct, SLOT(setEnabled(bool)));
    connect(textEdit, SIGNAL(copyAvailable(bool)),
            copyAct, SLOT(setEnabled(bool)));
}
//! [24]

//! [25] //! [26]
void MainWindow::createMenus()
//! [25] //! [27]
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
//! [28]
    fileMenu->addAction(openAct);
//! [28]
    fileMenu->addAction(saveAct);
//! [26]
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addSeparator();
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addSeparator();
    editMenu->addAction(selectAllAct);

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(zoomInAct);
    viewMenu->addAction(zoomOutAct);
    viewMenu->addSeparator();
    viewMenu->addAction(wordWrapAct);
    viewMenu->addSeparator();
    viewMenu->addAction(invertColoursAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}
//! [27]

//! [29] //! [30]
void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
//! [29] //! [31]
    fileToolBar->addAction(openAct);
//! [31]
    fileToolBar->addAction(saveAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);
}
//! [30]

//! [32]
void MainWindow::createStatusBar()
//! [32] //! [33]
{
    statusBar()->showMessage(tr("Ready"));
}
//! [33]

//! [34] //! [35]
void MainWindow::readSettings()
//! [34] //! [36]
{
    QSettings settings("presage", "qprompter");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);
}
//! [35] //! [36]

//! [37] //! [38]
void MainWindow::writeSettings()
//! [37] //! [39]
{
    QSettings settings("presage", "qprompter");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}
//! [38] //! [39]

//! [40]
bool MainWindow::maybeSave()
//! [40] //! [41]
{
    if (textEdit->isModified()) {
        int ret = QMessageBox::warning(this, tr("qprompter"),
				       tr("The document has been modified.\n"
					  "Do you want to save your changes?"),
				       QMessageBox::Yes | QMessageBox::Default,
				       QMessageBox::No,
				       QMessageBox::Cancel | QMessageBox::Escape);
        if (ret == QMessageBox::Yes)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}
//! [41]

//! [42]
void MainWindow::loadFile(const QString &fileName)
//! [42] //! [43]
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("qprompter"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    textEdit->setText(in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}
//! [43]

//! [44]
bool MainWindow::saveFile(const QString &fileName)
//! [44] //! [45]
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("qprompter"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    out << textEdit->text();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}
//! [45]

//! [46]
void MainWindow::setCurrentFile(const QString &fileName)
//! [46] //! [47]
{
    curFile = fileName;
    textEdit->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    setWindowFilePath(shownName);
}
//! [47]

//! [48]
QString MainWindow::strippedName(const QString &fullFileName)
//! [48] //! [49]
{
    return QFileInfo(fullFileName).fileName();
}
//! [49]
