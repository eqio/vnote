#ifndef VEXPORTDIALOG_H
#define VEXPORTDIALOG_H

#include <QDialog>
#include <QPageLayout>

#include "vconstants.h"

class QLabel;
class VLineEdit;
class QDialogButtonBox;
class QComboBox;
class QPushButton;
class QGroupBox;
class QPlainTextEdit;
class VNotebook;
class VDirectory;
class VFile;
class VCart;
class VExporter;
class QCheckBox;
class VLineEdit;


enum class ExportSource
{
    CurrentNote = 0,
    CurrentFolder,
    CurrentNotebook,
    Cart
};


enum class ExportFormat
{
    Markdown = 0,
    HTML,
    PDF
};


enum class ExportPageNumber
{
    None = 0,
    Left,
    Center,
    Right
};


struct ExportHTMLOption
{
    ExportHTMLOption()
        : m_embedCssStyle(true),
          m_completeHTML(true),
          m_mimeHTML(false)
    {
    }

    ExportHTMLOption(bool p_embedCssStyle,
                     bool p_completeHTML,
                     bool p_mimeHTML)
        : m_embedCssStyle(p_embedCssStyle),
          m_completeHTML(p_completeHTML),
          m_mimeHTML(p_mimeHTML)
    {
    }

    bool m_embedCssStyle;
    bool m_completeHTML;
    bool m_mimeHTML;
};


struct ExportPDFOption
{
    ExportPDFOption()
        : m_layout(NULL),
          m_wkhtmltopdf(false),
          m_wkEnableBackground(true),
          m_wkEnableTableOfContents(false),
          m_wkPageNumber(ExportPageNumber::None)
    {
    }

    ExportPDFOption(QPageLayout *p_layout,
                    bool p_wkhtmltopdf,
                    const QString &p_wkPath,
                    bool p_wkEnableBackground,
                    bool p_wkEnableTableOfContents,
                    ExportPageNumber p_wkPageNumber,
                    const QString &p_wkExtraArgs)
        : m_layout(p_layout),
          m_wkhtmltopdf(p_wkhtmltopdf),
          m_wkPath(p_wkPath),
          m_wkEnableBackground(p_wkEnableBackground),
          m_wkEnableTableOfContents(p_wkEnableTableOfContents),
          m_wkPageNumber(p_wkPageNumber),
          m_wkExtraArgs(p_wkExtraArgs)
    {
    }

    QPageLayout *m_layout;
    bool m_wkhtmltopdf;
    QString m_wkPath;
    bool m_wkEnableBackground;
    bool m_wkEnableTableOfContents;
    ExportPageNumber m_wkPageNumber;
    QString m_wkExtraArgs;
};


struct ExportOption
{
    ExportOption()
        : m_source(ExportSource::CurrentNote),
          m_format(ExportFormat::Markdown),
          m_renderer(MarkdownConverterType::MarkdownIt),
          m_processSubfolders(false)
    {
    }

    ExportOption(ExportSource p_source,
                 ExportFormat p_format,
                 MarkdownConverterType p_renderer,
                 const QString &p_renderBg,
                 const QString &p_renderStyle,
                 const QString &p_renderCodeBlockStyle,
                 bool p_processSubfolders,
                 const ExportPDFOption &p_pdfOpt,
                 const ExportHTMLOption &p_htmlOpt)
        : m_source(p_source),
          m_format(p_format),
          m_renderer(p_renderer),
          m_renderBg(p_renderBg),
          m_renderStyle(p_renderStyle),
          m_renderCodeBlockStyle(p_renderCodeBlockStyle),
          m_processSubfolders(p_processSubfolders),
          m_pdfOpt(p_pdfOpt),
          m_htmlOpt(p_htmlOpt)
    {
    }

    ExportSource m_source;
    ExportFormat m_format;
    MarkdownConverterType m_renderer;

    // Background name.
    QString m_renderBg;

    QString m_renderStyle;
    QString m_renderCodeBlockStyle;

    // Whether process subfolders recursively when source is CurrentFolder.
    bool m_processSubfolders;

    ExportPDFOption m_pdfOpt;

    ExportHTMLOption m_htmlOpt;
};


class VExportDialog : public QDialog
{
    Q_OBJECT
public:
    VExportDialog(VNotebook *p_notebook,
                  VDirectory *p_directory,
                  VFile *p_file,
                  VCart *p_cart,
                  MarkdownConverterType p_renderer,
                  QWidget *p_parent = nullptr);

private slots:
    void startExport();

    void handleOutputBrowseBtnClicked();

    void handleWkPathBrowseBtnClicked();

    void handleInputChanged();

    void handleLayoutBtnClicked();

    void handleCurrentFormatChanged(int p_index);

    void handleCurrentSrcChanged(int p_index);

private:
    void setupUI();

    QWidget *setupPDFAdvancedSettings();

    QWidget *setupHTMLAdvancedSettings();

    QWidget *setupGeneralAdvancedSettings();

    void initUIFields(MarkdownConverterType p_renderer);

    QString getOutputDirectory() const;

    void appendLogLine(const QString &p_text);

    // Return number of files exported.
    int doExport(VFile *p_file,
                 const ExportOption &p_opt,
                 const QString &p_outputFolder,
                 QString *p_errMsg = NULL);

    int doExport(VDirectory *p_directory,
                 const ExportOption &p_opt,
                 const QString &p_outputFolder,
                 QString *p_errMsg = NULL);

    int doExport(VNotebook *p_notebook,
                 const ExportOption &p_opt,
                 const QString &p_outputFolder,
                 QString *p_errMsg = NULL);

    int doExport(VCart *p_cart,
                 const ExportOption &p_opt,
                 const QString &p_outputFolder,
                 QString *p_errMsg = NULL);

    int doExportMarkdown(VFile *p_file,
                         const ExportOption &p_opt,
                         const QString &p_outputFolder,
                         QString *p_errMsg = NULL);

    int doExportPDF(VFile *p_file,
                    const ExportOption &p_opt,
                    const QString &p_outputFolder,
                    QString *p_errMsg = NULL);

    int doExportHTML(VFile *p_file,
                     const ExportOption &p_opt,
                     const QString &p_outputFolder,
                     QString *p_errMsg = NULL);

    // Return false if we could not continue.
    bool checkUserAction();

    void updatePageLayoutLabel();

    bool checkWkhtmltopdfExecutable(const QString &p_file);

    QComboBox *m_srcCB;

    QComboBox *m_formatCB;

    QComboBox *m_rendererCB;

    QComboBox *m_renderBgCB;

    QComboBox *m_renderStyleCB;

    QComboBox *m_renderCodeBlockStyleCB;

    VLineEdit *m_outputEdit;

    QGroupBox *m_basicBox;

    QGroupBox *m_settingBox;

    QWidget *m_pdfSettings;

    QWidget *m_htmlSettings;

    QWidget *m_generalSettings;

    QPlainTextEdit *m_consoleEdit;

    QDialogButtonBox *m_btnBox;

    QPushButton *m_openBtn;

    QPushButton *m_exportBtn;

    QLabel *m_layoutLabel;

    QCheckBox *m_wkhtmltopdfCB;

    VLineEdit *m_wkPathEdit;

    QPushButton *m_wkPathBrowseBtn;

    QCheckBox *m_wkBackgroundCB;

    QCheckBox *m_wkTableOfContentsCB;

    QComboBox *m_wkPageNumberCB;

    VLineEdit *m_wkExtraArgsEdit;

    QCheckBox *m_embedStyleCB;

    QCheckBox *m_completeHTMLCB;;

    QCheckBox *m_mimeHTMLCB;

    QCheckBox *m_subfolderCB;

    VNotebook *m_notebook;

    VDirectory *m_directory;

    VFile *m_file;

    VCart *m_cart;

    QPageLayout m_pageLayout;

    // Whether we are exporting files.
    bool m_inExport;

    // Asked to stop exporting by user.
    bool m_askedToStop;

    // Exporter used to export PDF and HTML.
    VExporter *m_exporter;

    // Last output folder path.
    static QString s_lastOutputFolder;

    static ExportOption s_opt;
};

#endif // VEXPORTDIALOG_H