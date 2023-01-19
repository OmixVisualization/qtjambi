/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of QtJambi.
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

import QtJambiGenerator 1.0

TypeSystem{
    packageName: "io.qt.qt3d.render.quick"
    defaultSuperClass: "io.qt.QtObject"
    module: "qtjambi.qt3dquickrender"
    description: ""
    NamespaceType{
        name: "Qt3DRender::Render::Quick"
        javaName: "Qt3DQuickRenderer"
        generate: false
    }
    
    ObjectType{
        name: "Qt3DRender::Render::Quick::Quick3DEffect"
        javaName: "Quick3DEffect"
        ExtraIncludes{
            Include{
                fileName: "QtJambiQml/hashes.h"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::Render::Quick::Quick3DMaterial"
        javaName: "Quick3DMaterial"
        ExtraIncludes{
            Include{
                fileName: "QtJambiQml/hashes.h"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::Render::Quick::Quick3DParameter"
        javaName: "Quick3DParameter"
        ExtraIncludes{
            Include{
                fileName: "QtJambiQml/hashes.h"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::Render::Quick::Quick3DRenderPass"
        javaName: "Quick3DRenderPass"
        ExtraIncludes{
            Include{
                fileName: "QtJambiQml/hashes.h"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::Render::Quick::Quick3DRenderPassFilter"
        javaName: "Quick3DRenderPassFilter"
        ExtraIncludes{
            Include{
                fileName: "QtJambiQml/hashes.h"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::Render::Quick::Quick3DRenderTarget"
        javaName: "Quick3DRenderTarget"
        ExtraIncludes{
            Include{
                fileName: "QtJambiQml/hashes.h"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::Render::Quick::Quick3DRenderTargetSelector"
        javaName: "Quick3DRenderTargetSelector"
        ExtraIncludes{
            Include{
                fileName: "QtJambiQml/hashes.h"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::Render::Quick::Quick3DScene"
        javaName: "Quick3DScene"
        ExtraIncludes{
            Include{
                fileName: "QtJambiQml/hashes.h"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::Render::Quick::Quick3DShaderData"
        javaName: "Quick3DShaderData"
        ExtraIncludes{
            Include{
                fileName: "QtJambiQml/hashes.h"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::Render::Quick::Quick3DShaderDataArray"
        javaName: "Quick3DShaderDataArray"
        ExtraIncludes{
            Include{
                fileName: "QtJambiQml/hashes.h"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::Render::Quick::Quick3DSortMethod"
        javaName: "Quick3DSortMethod"
        ExtraIncludes{
            Include{
                fileName: "QtJambiQml/hashes.h"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::Render::Quick::Quick3DStateSet"
        javaName: "Quick3DStateSet"
        ExtraIncludes{
            Include{
                fileName: "QtJambiQml/hashes.h"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::Render::Quick::Quick3DTechnique"
        javaName: "Quick3DTechnique"
        ExtraIncludes{
            Include{
                fileName: "QtJambiQml/hashes.h"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::Render::Quick::Quick3DTechniqueFilter"
        javaName: "Quick3DTechniqueFilter"
        ExtraIncludes{
            Include{
                fileName: "QtJambiQml/hashes.h"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::Render::Quick::Quick3DTextureExtension"
        javaName: "Quick3DTextureExtension"
        ExtraIncludes{
            Include{
                fileName: "QtJambiQml/hashes.h"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "Qt3DRender::Render::Quick::Quick3DViewport"
        javaName: "Quick3DViewport"
        ExtraIncludes{
            Include{
                fileName: "QtJambiQml/hashes.h"
                location: Include.Global
            }
        }
    }
}
