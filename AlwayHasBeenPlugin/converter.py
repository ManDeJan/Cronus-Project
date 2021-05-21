from cmath import inf
from PIL import Image
import cv2
import hitherdither

def to_int(r, g, b):
    return r << 16 | g << 8 | b

def hex_to_tup(rgb):
    r = rgb >> 16
    g = (rgb >> 8) & 0xFF
    b = rgb & 0xFF
    return r, g, b

def quantizetopalette(silf, palette, dither=False):
    """Convert an RGB or L mode image to use a given P image's palette."""

    silf.load()

    # use palette from reference image made below
    palette.load()
    im = silf.im.convert("P", 1, palette.im)
    # the 0 above means turn OFF dithering making solid colors
    return silf._new(im)

palette_og = [0, 723723, 789516, 855309, 921102, 986895, 1052688, 1513239, 1250841, 1382681, 1710618, 1842204, 1907997, 1973790, 2039583, 2105376, 2236962, 1515823, 2565927, 2631720, 2697513, 1911598, 2829099, 1320255, 2960685, 271438, 3026478, 3158064, 3223857, 3421236, 3487029, 3552822, 3684408, 3750201, 1849666, 3881787, 4013373, 4144959, 4210752, 602990, 4276545, 4342338, 4408131, 84305, 4539717, 590050, 4737096, 605052, 4802889, 4868682, 5000268, 1248224, 5131854, 1380066, 1511396, 5263440, 1577189, 5329233, 87907, 672140, 1972189, 5526612, 2037982, 5658198, 5723991, 5855577, 739226, 5921370, 2432740, 2695899, 5987163, 2564326, 6118749, 6184542, 91507, 6316128, 6381921, 6447714, 3419864, 741288, 3485657, 6579300, 3354085, 6645093, 3617243, 3683036, 6842472, 3551464, 4143573, 6908265, 29569, 4012001, 1005239, 7171437, 7303023, 4275173, 4867538, 7368816, 7434609, 7500402, 941508, 4604138, 7631988, 7697781, 4933345, 7763574, 5591247, 7895160, 33171, 1073873, 8092539, 5854419, 5525226, 5986261, 8289918, 6249421, 8355711, 5591275, 6183384, 5985763, 8487297, 5920227, 8553090, 8618883, 1010144, 6183398, 36514, 8750469, 6973386, 9013641, 6973400, 1077227, 9145227, 6643949, 7697095, 9211020, 7038693, 9276813, 9342606, 7170279, 9408399, 7367902, 39599, 9474192, 9539985, 1209848, 9605778, 8421060, 9737364, 7960281, 9803157, 9868950, 9934743, 7696623, 8157660, 8091367, 10066329, 9144769, 42943, 10263708, 9276100, 10329501, 9078734, 10461087, 10526880, 8683504, 9605320, 9802942, 10724259, 8947176, 10790052, 10855845, 10921638, 10987431, 11053224, 11119017, 10526908, 46799, 11316396, 10855616, 9605106, 11447982, 9934057, 11513775, 11250873, 11711154, 11447996, 49628, 11908533, 11316424, 12040119, 12105912, 10855401, 10657780, 11513303, 12237498, 11776717, 12303291, 12369084, 11842768, 12500670, 12566463, 52714, 12632256, 12698049, 11842282, 12961221, 11710454, 13158600, 13224393, 13290186, 13355979, 56312, 12763370, 13487565, 13553358, 13619151, 57342, 12697591, 12895470, 13816530, 14013909, 14145495, 13619179, 14277081, 14408667, 13750265, 14474460, 14606046, 14671839, 14606060, 14935011, 15000804, 15132390, 15263976, 14671867, 15395562, 15592941, 15724527, 15856113, 15724541, 16185078, 16448250, 16777215, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ]
palette = [0x00609C, 0x00619B, 0x01619E, 0x02639E, 0x0564A2, 0x0C65A3, 0x0D649F, 0x12639B, 0x1266A4, 0x1369A5, 0x1469A8, 0x1A6AA3, 0x1B669F, 0x1C67A3, 0x1C6BA8, 0x1F659C, 0x1F689D, 0x206BA4, 0x21679E, 0x21699E, 0x226EA9, 0x23689F, 0x246AA4, 0x256EAA, 0x2671AB, 0x286AA0, 0x2A699D, 0x2A6AA3, 0x2A6CA8, 0x2A6FA8, 0x2A6FAC, 0x2B6DA2, 0x2B72AC, 0x2C6FA5, 0x2C73AF, 0x3273AE, 0x3372A9, 0x3771A6, 0x3778B3, 0x3874A9, 0x3972A4, 0x3976B1, 0x3979B0, 0x3B75AC, 0x3B78AC, 0x3C72A1, 0x3C7AAD, 0x3D76A6, 0x3E719D, 0x3E75A9, 0x417BB3, 0x417FB3, 0x427CB0, 0x4280B7, 0x437AAC, 0x4978A4, 0x497AA9, 0x497EB0, 0x4B7CAC, 0x4C81B4, 0x4D83B8, 0x4F7599, 0x4F80B0, 0x5080AD, 0x517CA9, 0x547CA4, 0x5485B1, 0x5681B0, 0x5780A4, 0x5885B4, 0x5886B7, 0x588DBE, 0x5982AA, 0x5982AC, 0x598BB9, 0x5A8AB6, 0x5B86B1, 0x5E7A97, 0x6082A3, 0x6084AC, 0x618BB6, 0x618FBC, 0x6488B1, 0x6789AC, 0x69849D, 0x6987A6, 0x6989A9, 0x698CB6, 0x6990BA, 0x6993BF, 0x6A8DB1, 0x6B90B6, 0x6E9BC6, 0x7099BF, 0x7193BC, 0x738DAD, 0x7392B6, 0x748EA9, 0x748EAA, 0x758EA8, 0x7591AE, 0x7599BA, 0x769EC6, 0x77899C, 0x778DA4, 0x7795BB, 0x779ABF, 0x7A92AB, 0x7B92AE, 0x7B94AE, 0x7C9ABA, 0x7C9EC3, 0x7D95B5, 0x7D9BBF, 0x7DA2C9, 0x7F96B1, 0x809AB7, 0x8297AF, 0x839DBF, 0x8498B2, 0x859AB6, 0x85A2C0, 0x85A2C4, 0x85A5C8, 0x85A7CC, 0x85ABD3, 0x889AB6, 0x889DB7, 0x8C9AAF, 0x8C9EB8, 0x8D9FB6, 0x8DA0BA, 0x8DA2C1, 0x8E9FB5, 0x8EA6C5, 0x8FA1BD, 0x8FA5C0, 0x90A0B8, 0x91A2BB, 0x91AAC6, 0x92ACCA, 0x92AECD, 0x93A1B0, 0x93A6C0, 0x93B0D1, 0x96A5BB, 0x96B3D3, 0x97A7C0, 0x97AEC7, 0x98AAC2, 0x98ABC5, 0x99AABE, 0x99ABC0, 0x99B8DB, 0x9BADC2, 0x9BAFC6, 0x9CB1CD, 0x9CB7D6, 0x9DB0CA, 0x9EB3CA, 0x9FADC0, 0x9FB0C4, 0xA0B4D0, 0xA1B1C7, 0xA1B6CE, 0xA1B8D1, 0xA2B9D3, 0xA2C3E4, 0xA3AFC1, 0xA3B5CB, 0xA5B3C3, 0xA5B5C7, 0xA7B7CB, 0xA7BAD2, 0xA9BACE, 0xA9C0DA, 0xABBACB, 0xABBFD1, 0xABC6E4, 0xADB6C2, 0xADB8C8, 0xADBBD2, 0xAFBCCE, 0xAFBFD0, 0xAFC1D6, 0xB0BBCB, 0xB0C4DD, 0xB1C3D9, 0xB1CDE8, 0xB2C0D1, 0xB5C6DC, 0xB6C0CE, 0xB6C1D3, 0xB6C4D7, 0xB6C7D8, 0xB6CDE3, 0xB7C4D3, 0xBAC5D7, 0xBAC9D7, 0xBAC9DA, 0xBACADE, 0xBACDDE, 0xBAD6F1, 0xBEC5D2, 0xBEC8D7, 0xBEC9DA, 0xBFCCDA, 0xC0CEDD, 0xC3D8F0, 0xC4CFE0, 0xC5CAD7, 0xC5CEDD, 0xC6D2DD, 0xC7CEDA, 0xC7D6E8, 0xC8D2E0, 0xC8D3E2, 0xC8D4E4, 0xC9D5E5, 0xC9DEF3, 0xCAD3DE, 0xCBD6E0, 0xCBD6E2, 0xCED7E3, 0xCEECFD, 0xCFD5DE, 0xD0F4FE, 0xD1D9E5, 0xD2D9E2, 0xD2DAE7, 0xD3DDE6, 0xD4DFED, 0xD6DEE7, 0xD6E0EB, 0xD8DEE6, 0xD8EFFA, 0xD9E1EC, 0xDAE1E9, 0xDAE4EC, 0xDCE1EC, 0xDCE6EE, 0xDCEEF8, 0xDDE4EC, 0xDEE5EE, 0xE0E5ED, 0xE3E8EF, 0xE3E8F1, 0xE6F8FE, 0xE7EBF2, 0xE9EDF5, 0xE9EEF1, 0xEAF0F8, 0xEFF1F6, 0xF1F6F9, 0xF5F7FB, 0xFFFFFF]
palette = list(map(hex_to_tup, palette))
cap = cv2.VideoCapture('../../Cronus/Title/always_has_been.mp4')

def get_closest_color_idx(color):
    r1, g1, b1 = color
    i = 0
    lowest_d = inf
    idx = -1
    for r2, g2, b2 in palette:
        d = (abs(r2 - r1) * 0.30) ** 2 + \
            (abs(g2 - g1) * 0.59) ** 2 + \
            (abs(b2 - b1) * 0.11) ** 2
        if d < lowest_d:
            lowest_d = d
            idx = i

        i += 1
    # print(idx)
    return idx

idx_bytes = []
for i in range(482):
    print(i)
    succes, image = cap.read()
    # img_dithered = hitherdither.diffusion.error_diffusion_dithering(image, palette)
    pil_image = Image.fromarray(cv2.cvtColor(image, cv2.COLOR_BGR2RGB))
    pil_image.save('out.png')
    pil_image.convert()
    palimage = Image.new('P', (16, 16))
    palimage.putpalette([item for sublist in palette for item in sublist])
    # oldimage = Image.open(sys.argv[1])
    # oldimage = oldimage.convert("RGB")
    pilimage = quantizetopalette(pil_image, palimage, dither=False)
    # dirname, filename= os.path.split(imgfn)
    # name, ext= os.path.splitext(filename)
    # newpathname= os.path.join(dirname, "cga-%s.png" % name)
    # newimage.save(newpathname)
    pilimage.save('dith.png')

    for y in range(240):
        for x in range(320):
            pixel = pilimage.getpixel((x, y))
            # idx = get_closest_color_idx(pixel)
            # print(pixel, hex(idx), end=': ')
            # print(palette[idx])
            idx_bytes.append(pixel)
    with open('../alwaysraw', 'wb') as outfile:
        outfile.write(bytes(idx_bytes))