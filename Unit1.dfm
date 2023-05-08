object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Entrenamiento'
  ClientHeight = 227
  ClientWidth = 326
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  TextHeight = 15
  object Label1: TLabel
    Left = 16
    Top = 8
    Width = 189
    Height = 15
    Caption = 'Introduce la ruta del txt de  archivos'
  end
  object Label2: TLabel
    Left = 24
    Top = 80
    Width = 197
    Height = 15
    Caption = 'Intorduce la ruta del archivo de pesos'
  end
  object txt_path: TEdit
    Left = 24
    Top = 40
    Width = 225
    Height = 23
    TabOrder = 0
    TextHint = 'TXT PATH'
  end
  object weight_data: TEdit
    Left = 24
    Top = 112
    Width = 225
    Height = 23
    TabOrder = 1
    TextHint = 'WEIGHT TXT'
  end
  object process: TButton
    Left = 24
    Top = 176
    Width = 121
    Height = 41
    Caption = 'Procesar'
    TabOrder = 2
    OnClick = processClick
  end
  object Button1: TButton
    Left = 184
    Top = 177
    Width = 118
    Height = 39
    Caption = 'Display Image'
    TabOrder = 3
    OnClick = Button1Click
  end
end
